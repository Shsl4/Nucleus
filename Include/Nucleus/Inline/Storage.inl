#pragma once

#ifdef STORAGE_INLINE

namespace Nucleus {

    template <typename Integral> requires std::is_integral_v<Integral>
    Integral Value::get() const {
        try {
            return static_cast<Integral>(value.toInteger());
        }
        catch (std::exception const&) {
            throw Exceptions::Exception("Deserialization error");
        }
    }

    template <typename Fp> requires std::is_floating_point_v<Fp>
    Fp Value::get() const {

        try {
            return static_cast<Fp>(value.toFloat64());
        }
        catch (std::exception const&) {
            throw Exceptions::Exception("Deserialization error");
        }
            
    }
    
    template <class T>
    void Value::store(T const& element) {
        value = Fmt<T>::format(element, "q");
    }
    
}

#endif