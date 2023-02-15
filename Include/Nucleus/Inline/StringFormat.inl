#pragma once

#ifdef STRING_INLINE

#include <cmath>
#include <Nucleus/Fmt.h>

namespace Nucleus {
    
    template<typename T, typename... Args> requires HasFmt<T>::value
    void expand(MutableArray<String>& storage, T const& element, Args&& ... args) {
                
        storage += Fmt<T>::format(element);
        
        if (sizeof...(args) > 0) expand(storage, args...);
        
    }
    
    template<typename T> requires HasFmt<T>::value
    void expand(MutableArray<String>& storage, T const& element) {
        
        storage += Fmt<T>::format(element);
        
    }
    
    inline void expand(MutableArray<String>&) { }
    
    template<typename... Args>
    String String::format(String const& fmt, Args&&... args) {

        MutableArray<String> arguments(sizeof...(args));
        
        // If no matching function is found, T must implement a formatter.
        expand(arguments, args...);

        if(arguments.size() == 0) return fmt;
        
        return internalFormat(fmt, arguments);
        
    }

    template <typename T>
    String String::fromInteger(T const& integral) {
            
        static_assert(std::is_integral_v<T>, "T must be an integral type");

        if (integral == static_cast<T>(0)) return "0";

        // Static format buffer.
        static char buf[100];
            
        T n = integral;
        size_t digits = integral < 0 ? 1 : 0;
        while (n) { n /= 10; ++digits; }

        std::to_chars(buf, buf + 100, integral, 10);

        return { buf, digits };
            
    }

    template <typename T>
    String String::fromFloatingPoint(T const& fp) {

        static_assert(std::is_floating_point_v<T>, "T must be an integral type");

        return std::to_string(fp).c_str();
            
    }

    template <typename T>
    String String::fromPointer(const T* pointer) {

        if(!pointer) { return "nullptr"; }
            
        const auto address = reinterpret_cast<intptr_t>(pointer);
            
        // Static format buffer.
        static char buf[100];
            
        std::to_chars(buf, buf + 100, address, 16);
            
        const String strAddress(buf, strlen(buf));
            
        return "0x" + strAddress;
            
    }

}

#endif
