#pragma once

#include <Nucleus/String.h>
#include <Nucleus/Exceptions.h>

#include "Archive.h"

namespace Nucleus {
    
    template<class T>
    class Serializer;
    
    class Json {
    
    public:
        
        enum class DataType {
            String,
            Object,
            Boolean,
            Number,
            Null,
            List
        };

        struct Object;

        struct Field {
            
            Field() = default;

            Field(String name, String data, DataType type);

            bool operator==(const Field& other) const;

            void write(String& str, size_t indent) const;

        private:

            friend Object;
            
            String name = {};
            String data = {};
            DataType type = DataType::Null;
            
        };
        
        struct Object {
            
            Object() = default;

            explicit Object(String name);

            bool operator==(const Object& other) const;
            
            template<class T>
            void add(String const& name, T const& object) {
                auto newObject = Object(name);
                Serializer<T>::serialize(newObject, object);
                objects += newObject;
            }

            template<template<typename> typename CollectionType, typename T> requires (std::is_base_of_v<Collection<T>, CollectionType<T>> && HasFmt<T>::value)
            void add(String const& name, CollectionType<T> const& collection) {
                fields += Field(name, Fmt<CollectionType<T>>::format(collection, ""), DataType::List);
            }

            template<template<typename> typename CollectionType, typename T> requires (std::is_base_of_v<Collection<T>, CollectionType<T>> && !HasFmt<T>::value)
            void add(String const& name, CollectionType<T> const& collection) {
                
                String str = "[ ";
                for (size_t i = 0; i < collection.size() - 1; ++i) {
                    Object obj;
                    Serializer<T>::serialize(obj, collection[i]);
                    obj.write(str, 0, false);
                    str += ", ";
                }
                
                Object obj;
                Serializer<T>::serialize(obj, collection[collection.size() - 1]);
                obj.write(str, 0, false);
                
                str += " ]";

                str.removeOccurrences("\n");

                fields += Field(name, str, DataType::List);

                
            }

            template<class T> requires (std::is_integral_v<T> || std::is_floating_point_v<T>)
            void add(String const& name, T const& numeric) {
                fields += Field(name, Fmt<Float64>::format(f64(numeric), "1"), DataType::Number);
            }

            void add(String const& name, const bool value) {
                fields += Field(name, Fmt<bool>::format(value, ""), DataType::Boolean);
            }

            void add(Object const& object) {
                nthrowif(object.name.isEmpty(), "Object name must not be empty");
                objects += object;
            }

            void add(String const& name, nullptr_t) {
                fields += Field(name, "null", DataType::Null);
            }

            void add(String const& name, String const& str) {
                fields += Field(name, str, DataType::String);
            }

            bool getBool(String const& name) const;
            
            String getString(String const& name) const;

            template<class T>
            T getObject(String const& name) const {
                
                for (auto const& object : objects) {
                    
                    if(object.name == name) {
                        
                        return Serializer<T>::deserialize(object);
                        
                    }
                    
                }

                throw Exceptions::BadType("The variable does not exist.");
                
            }

            template<class T> requires (std::is_integral_v<T>)
            T getInteger(String const& name) const {

                for (auto const& field : fields) {
                    
                    if(field.name == name) {
                        
                        if (field.type == DataType::Number) {

                            return static_cast<T>(field.data.toFloat64());
                            
                        }

                        throw Exceptions::BadType("The variable is not an integer.");
                        
                    }
                    
                }

                throw Exceptions::BadType("The variable does not exist.");
                
            }

            template<class T> requires (std::is_floating_point_v<T>)
            T getFloat(String const& name) const {

                for (auto const& field : fields) {
                    
                    if(field.name == name) {
                        
                        if (field.type == DataType::Number) {

                            return static_cast<T>(field.data.toFloat64());
                            
                        }

                        throw Exceptions::BadType("The variable is not a float.");
                        
                    }
                    
                }

                throw Exceptions::BadType("The variable does not exist.");
                
            }
            
            template<class T> requires std::is_integral_v<T>
            static MutableArray<T> deserializeArray(String const& data) {

                auto elements = data.substring(1, data.size() - 1).removeOccurrences(" ").split(",");
                MutableArray<T> retVal = {};

                for (auto const& e : elements) {
                    Int64 i;
                    e.toInteger(i);
                    retVal += static_cast<T>(i);
                }

                return retVal;
                    
                
            }

            template<class T> requires std::is_floating_point_v<T>
            static MutableArray<T> deserializeArray(String const& data) {
                
                const auto elements = data.substring(1, data.size() - 1).removeOccurrences(" ").split(",");
                
                MutableArray<T> retVal = {};

                for (auto const& e : elements) {
                    retVal += static_cast<T>(e.toFloat64());
                }

                return retVal;
                
            }

            template<class T>
            static MutableArray<T> deserializeArray(String const& data) {
                
                const auto elements = data.substring(1, data.size() - 1).removeOccurrences(" ").split(",");
                
                MutableArray<T> retVal = {};

                for (auto const& e : elements) {
                    Object object;
                    parseObject(e, object);
                    retVal += Serializer<T>::deserialize(object);
                }

                return retVal;
                
            }

            template<>
            static MutableArray<String> deserializeArray(String const& data) {
                
                return data.substring(1, data.size() - 1).removeOccurrences(" ").split(",");
                
            }
            
            template<class T>
            MutableArray<T> getArray(String const& name) const {

                for (auto const& field : fields) {
                    
                    if(field.name == name) {
                        
                        if (field.type == DataType::List) {

                            return deserializeArray<T>(field.data);
                            
                        }

                        throw Exceptions::BadType("The variable is not an array.");
                        
                    }
                    
                }

                throw Exceptions::BadType("The variable does not exist.");
                
            }

        private:

            friend class Json;

            void write(String& str, size_t indent, bool useIndent = true) const;
            
            String name = {};
            MutableArray<Field> fields = {};
            MutableArray<Object> objects = {};
            
        };
        
        static Json parse(String const& fileName);

        NODISCARD void write(String const& fileName) const;

        Object rootObject = Object("");

    private:
        
        static DataType nextType(String const& data, size_t from);

        static void parseObject(String const& data, Object& storage);
        
        static void nextObject(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextList(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextString(String const& data, size_t start, size_t& begin, size_t& end);

        static void nextBool(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextNull(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextNumber(String const& data, size_t from, size_t& begin, size_t& end);
    
    };

}
