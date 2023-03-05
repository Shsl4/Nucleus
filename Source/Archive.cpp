#include <Nucleus/Archive.h>

namespace Nucleus{
    
    DataArray* Container::createArray(String const& name) {

        DataArray* container = Allocator<DataArray>::construct(name);
        elementArray.add(container);
        return container;
            
    }

    Container* Container::createContainer(DataArray* arr) {
        return arr->createContainer();
    }

    Element* Container::findElement(String const& name) const {

        for (auto* element : elementArray) {
            if (element->name() == name) {
                return element;
            }
        }

        return nullptr;
            
    }

    void Archive::reset() {
        this->root = Unique<Container>::make();
    }
}
