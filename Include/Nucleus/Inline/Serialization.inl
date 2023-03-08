#pragma once

#ifdef SERIALIZATION_INLINE

namespace Nucleus {

template<class T>
class Serializer<MutableArray<T>> {

    public:

        static void serialize(Container* container, MutableArray<T> const& object) {
        
            for (auto const& e : object) {
                container->add(nullptr, e);
            }
            
        }
        
        static MutableArray<T> deserialize(Container* container) requires Serializable<T> {

            if(const auto arr = dynamic_cast<ArrayContainer*>(container)) {

                MutableArray<T> returnValue = {};
                
                for (auto const& storage : arr->dataArray()) {

                    if(const auto sub = dynamic_cast<Container*>(storage)) {
                        
                        returnValue.add(Serializer<T>::deserialize(sub));
                        
                    }
                    
                }

                return returnValue;
                
            }

            throw Exceptions::Exception("Deserialization error");
            
        }
        
        static MutableArray<T> deserialize(Container* container) requires (!Serializable<T>) {

            if(const auto arr = dynamic_cast<ArrayContainer*>(container)) {

                MutableArray<T> returnValue = {};
                
                for (auto const& storage : arr->dataArray()) {

                    if(const auto value = dynamic_cast<Value*>(storage)) {
                        
                        returnValue.add(value->get<T>());
                        
                    }
                    
                }

                return returnValue;
                
            }

            throw Exceptions::Exception("Deserialization error");
            
        }
        
    };
    
}

#endif
