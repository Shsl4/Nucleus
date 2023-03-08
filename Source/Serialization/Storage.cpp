#include <Nucleus/Serialization/Storage.h>

namespace Nucleus {
    
    Value* Value::duplicate() const {
        return Allocator<Value>::construct(*this);
    }

    
    template <>
    String Value::get<String>() const {
        return value.substring(1, value.size() - 1);
    }

    template <>
    bool Value::get<bool>() const {
        try {
            return value.toBool();
        }
        catch (std::exception const&) {
            throw Exceptions::Exception("Deserialization error");
        }
    }
    
}
