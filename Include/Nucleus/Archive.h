#pragma once

#include <Nucleus/String.h>
#include <Nucleus/List.h>

#include <utility>

#include "Exceptions.h"
#include "Function.h"
#include "SmartArray.h"

namespace Nucleus {
    
    template<class T>
    class Serializer;
    
    template <class T>
    using HasSerializer = decltype(implements(std::declval<Serializer<T>*>()));
            
    class Element {

    public:

        Element() = default;

        explicit Element(String name) : elementName(std::move(name)) {
            
        }

        bool operator==(const Element& other) const {
            return this == &other;
        }

        NODISCARD FORCEINLINE String name() const { return this->elementName; }

        NODISCARD FORCEINLINE virtual String stringValue() const = 0;
        
        virtual ~Element() = default;
        
    private:

        String elementName = "";
        
    };

    template<typename T> requires HasFmt<T>::value
    class Field final : public Element {
    
    public:
        
        Field() = default;
        
        explicit Field(String const& name, T const& data) : Element(name), data(data) {
            
        }
        
        NODISCARD FORCEINLINE T value() const { return this->data; }
        
        NODISCARD FORCEINLINE String stringValue() const override { return Fmt<T>::format(data, ""); }
        
    private:

        T data = {};
        
    };

    class DataArray;

    class Container : public Element {

    public:

        Container() = default;

        explicit Container(String const& name) : Element(name) {

        }

        NODISCARD FORCEINLINE String stringValue() const override { return "{ Container }"; }
        
        template<class T> requires !HasFmt<T>::value
        T get(String const& name) {

            Element* element = findElement(name);

            if (auto container = dynamic_cast<Container*>(element)) {
                
                return Serializer<T>::deserialize(container);
                
            }

            throw Exceptions::Exception("Deserialization error.");
            
        }

        template<class T> requires HasFmt<T>::value
        MutableArray<T> getArray(String const& name) {

            Element* element = findElement(name);

            if (auto field = dynamic_cast<Field<T>*>(element)) {
                
                return field->value();
                
            }

            throw Exceptions::Exception("Deserialization error.");
            
        }

        template<class T>
        MutableArray<T> getArray(String const& name) {

            Element* element = findElement(name);
            
            if (const auto array = dynamic_cast<Container*>(element)) {

                auto storage = MutableArray<T>(array->elements().size());
                
                for (auto* e : array->elements()) {

                    storage += Serializer<T>::deserialize(dynamic_cast<Container*>(e));
                    
                }

                return storage;
                
            }

            throw Exceptions::Exception("Deserialization error.");
            
        }

        template<class T>
        T get(String const& name) {

            Element* element = findElement(name);

            if (auto field = dynamic_cast<Field<T>*>(element)) {
                
                return field->value();
                
            }

            throw Exceptions::Exception("Deserialization error.");
            
        }

        template<class T> requires HasFmt<T>::value
        Field<T>* add(String const& name, T const& value) {

            Field<T>* field = Allocator<Field<T>>::construct(name, value);
            elementArray.add(field);
            return field;
            
        }

        template<class T> requires !HasFmt<T>::value
        Container* add(String const& name, T const& value) {

            Container* container = createContainer(name);
            Serializer<T>::serialize(container, value);
            return container;
            
        }
        
        template<template<typename> typename CollectionType, typename T>
        requires (std::is_base_of_v<Collection<T>, CollectionType<T>> && !HasFmt<T>::value)
        DataArray* add(String const& name, CollectionType<T> const& value) {

            DataArray* array = createArray(name);

            for (auto const& e : value) {
                Serializer<T>::serialize(createContainer(array), e);
            }
            
            return array;
            
        }
        
        Container* createContainer(String const& name = "") {

            Container* container = Allocator<Container>::construct(name);
            elementArray.add(container);
            return container;
            
        }
        
        DataArray* createArray(String const& name);

        NODISCARD FORCEINLINE SmartArray<Element> const& elements() const { return this->elementArray; }
        
    private:

        static Container* createContainer(DataArray*);
        
        NODISCARD Element* findElement(String const& name) const;

        SmartArray<Element> elementArray = {};
        
    };

    class DataArray : public Container {

    public:

        explicit DataArray(String const& name) : Container(name) {

        }
        
    };
    
    class Archive {

    public:

        Archive() = default;

        Archive(Archive const& other) {

            root = Unique<Container>::make(*other.rootContainer());
            
        }

        void reset();

        NODISCARD FORCEINLINE Container* rootContainer() const { return this->root.pointer(); }
        
    private:

        Unique<Container> root = Unique<Container>::make();
    
    };
    
}
