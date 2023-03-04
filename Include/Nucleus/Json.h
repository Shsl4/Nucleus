#pragma once

#include <Nucleus/String.h>

namespace Nucleus {
    
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

        struct Field {
            
            Field() = default;

            Field(String name, String data, DataType type);

            bool operator==(const Field& other) const;

            void write(String& str, size_t indent) const;

            String name = {};
            String data = {};
            DataType type = DataType::Null;
            
        };
        
        struct Object {

            Object() = default;

            explicit Object(String name);

            bool operator==(const Object& other) const;

            void write(String& str, size_t indent) const;

            String name = {};
            MutableArray<Field> fields = {};
            MutableArray<Object> objects = {};
            
        };
        
        static Json parse(String const& fileName);

        NODISCARD void write(String const& fileName) const;

        Object rootObject = Object("");

    private:
        
        static DataType nextType(String const& data, size_t from);

        static void parseNextObject(String const& data, Object& storage);
        
        static void nextObject(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextList(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextString(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextBool(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextNull(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextNumber(String const& data, size_t from, size_t& begin, size_t& end);
    
    };

}
