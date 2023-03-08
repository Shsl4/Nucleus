#pragma once

#include <Nucleus/Serialization/Storage.h>
#include <Nucleus/SmartArray.h>

namespace Nucleus {
    
    class Container : public Storage {

    public:
        
        template<typename T> requires HasFmt<T>::value
        void add(String const& name, T const& element);

        template<typename T> requires (!HasFmt<T>::value)
        void add(String const& name, T const& element);

        template<template<class T> class Array, typename T>
        void add(String const& name, Array<T> const& element);

        template<class T>
        T get(String const& name) const;

        template<typename T> requires (!HasFmt<T>::value)
        T get(String const& name) const;

        template<typename T> requires (!std::is_same_v<Lowest<T>, T>)
        T get(String const& name) const;

        Container* objectContainer(String const& name);
        
        Container* arrayContainer(String const& name);
    
    protected:

        NODISCARD virtual Value* findValue(String const& name) const = 0;

        NODISCARD virtual Container* findContainer(String const& name) const = 0;

        NODISCARD virtual Container* findArrayContainer(String const& name) const = 0;

        virtual Storage* store(String const&, Storage* storage) = 0;

    private:

        NODISCARD SmartArray<Storage> const& storageFromArray(String const& str) const;
        
    };

    class ArrayContainer final : public Container {

    public:

        ArrayContainer() = default;

        NODISCARD FORCEINLINE SmartArray<Storage> const& dataArray() const { return this->data; }

        NODISCARD ArrayContainer* duplicate() const override;

    protected:
        
        NODISCARD Value* findValue(String const& name) const override { return nullptr; }
        
        NODISCARD Container* findContainer(String const& name) const override { return nullptr; }

        NODISCARD Container* findArrayContainer(String const& name) const override { return nullptr; }

        Storage* store(String const&, Storage* storage) override;

    private:

        SmartArray<Storage> data;
        
    };


    class ObjectContainer final : public Container {

    public:

        ObjectContainer() = default;

        ~ObjectContainer() override;

        NODISCARD ObjectContainer* duplicate() const override;

        NODISCARD FORCEINLINE Map<String, Storage*> const& dataMap() const { return this->data; }

    protected:
        
        NODISCARD Value* findValue(String const& name) const override;

        NODISCARD Container* findContainer(String const& name) const override;

        NODISCARD Container* findArrayContainer(String const& name) const override;

        Storage* store(String const& name, Storage* storage) override;

    private:

        Map<String, Storage*> data;
        
    };
    
}

#define CONTAINER_INL
#include <Nucleus/Inline/Container.inl>
#undef CONTAINER_INL
