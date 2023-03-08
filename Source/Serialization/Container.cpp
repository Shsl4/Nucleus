#include <Nucleus/Serialization/Container.h>

namespace Nucleus {
    
    ArrayContainer* ArrayContainer::duplicate() const {

        ArrayContainer* object = Allocator<ArrayContainer>::construct();
            
        for (const auto& value : data) {

            object->data.add(value->duplicate());
                
        }

        return object;

    }

    Storage* ArrayContainer::store(String const&, Storage* storage) {
        data.add(storage);
        return storage;
    }

    ObjectContainer::~ObjectContainer() {

        for (auto& e : data) {
            Allocator<Storage>::destroy(e.value());
        }

        data.reset();
            
    }

    ObjectContainer* ObjectContainer::duplicate() const {

        ObjectContainer* object = Allocator<ObjectContainer>::construct();
            
        for (auto& entry : data) {

            object->data.add(entry.key(), entry.value()->duplicate());
                
        }

        return object;

    }

    Value* ObjectContainer::findValue(String const& name) const {
            
        try {
            return dynamic_cast<Value*>(data[name]);
        }
        catch (std::exception&) {
            return nullptr;
        }
            
    }

    Container* ObjectContainer::findContainer(String const& name) const {
            
        try {
            return dynamic_cast<Container*>(data[name]);
        }
        catch (std::exception&) {
            return nullptr;
        }
            
    }

    Container* ObjectContainer::findArrayContainer(String const& name) const {

        try {
            return dynamic_cast<ArrayContainer*>(data[name]);
        }
        catch (std::exception&) {
            return nullptr;
        }
            
    }

    Storage* ObjectContainer::store(String const& name, Storage* storage) {
            
        nthrowif(!data.add(name, storage), "A container cannot have multiple variables with the same name");
        return storage;
            
    }

    Container* Container::objectContainer(String const& name) {

        auto* newContainer = Allocator<ObjectContainer>::construct();

        store(name, newContainer);
            
        return newContainer;
        
    }
    
    Container* Container::arrayContainer(String const& name) {

        ArrayContainer* newContainer = Allocator<ArrayContainer>::construct();

        store(name, newContainer);
            
        return newContainer;
        
    }
    
    SmartArray<Storage> const& Container::storageFromArray(String const& str) const {

        if(const auto container = dynamic_cast<ArrayContainer*>(findArrayContainer(str))) {
            return container->dataArray();
        }

        throw Exceptions::Exception("");
        
    }
    
    
}
