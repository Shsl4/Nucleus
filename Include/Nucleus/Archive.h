#pragma once

#include <Nucleus/String.h>
#include <Nucleus/List.h>

#include <utility>

namespace Nucleus {

    class Element {

    public:

        Element() = default;

        explicit Element(String name) : elementName(std::move(name)) {
            
        }

        bool operator==(const Element& other) const {
            return this == &other;
        }

        NODISCARD FORCEINLINE String name() const { return this->elementName; }
        
    private:

        String elementName = "";
        
    };

    class Field : public Element {
    
    public:
        
        Field() = default;
        
        explicit Field(String const& name) : Element(name) {
            
        }
        
        String value() const { return this->data; }
        
    private:

        String data = {};
        
    };

    class Container : public Element {

    public:

        Container() = default;

        explicit Container(String const& name) : Element(name) {

        }

        template<class T> requires std::is_base_of_v<Element, T>
        T* createElement(String const& name) {

            elements.add(T(name));
            return elements[elements.size() - 1];
            
        }
        
    private:

        List<Element> elements = {};
        
    };
    
    class Archive {
        
    public:
    
    };
    
}
