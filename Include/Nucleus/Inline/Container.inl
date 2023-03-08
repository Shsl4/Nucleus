#pragma once

#ifdef CONTAINER_INL

namespace Nucleus {

    
    template <typename T> requires HasFmt<T>::value
    void Container::add(String const& name, T const& element) {

        auto* value = Allocator<Value>::construct();
        store(name, value);
        value->store(element);
            
    }

    template <typename T> requires (!HasFmt<T>::value)
    void Container::add(String const& name, T const& element) {
            
        Container* container = objectContainer(name);
        Serializer<T>::serialize(container, element);
            
    }

    template <template <class T> class Array, typename T>
    void Container::add(String const& name, Array<T> const& element) {

        Container* container = arrayContainer(name);

        Serializer<Array<T>>::serialize(container, element);
            
    }

    template <class T>
    T Container::get(String const& name) const {
            
        if(const Value* val = findValue(name)) {
            return val->get<T>();
        }

        throw Exceptions::Exception("Deserialization error");
            
    }

    template <typename T> requires (!HasFmt<T>::value)
    T Container::get(String const& name) const {
            
        if(Container* container = findContainer(name)) {
            return Serializer<T>::deserialize(container);
        }
            
        throw Exceptions::Exception("Deserialization error");
            
    }

    template <typename T> requires (!std::is_same_v<Lowest<T>, T>)
    T Container::get(String const& name) const {
            
        if(Container* container = findArrayContainer(name)) {
                
            return Serializer<T>::deserialize(container);
        }
            
        throw Exceptions::Exception("Deserialization error");
            
    }
    
}

#endif
