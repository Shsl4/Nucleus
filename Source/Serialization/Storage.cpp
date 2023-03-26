#include <Nucleus/Serialization/Storage.h>

namespace Nucleus {
    
    Value* Value::duplicate() const {
        return Allocator<Value>::construct(*this);
    }

}
