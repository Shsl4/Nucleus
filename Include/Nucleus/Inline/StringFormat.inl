#pragma once

#ifdef STRING_INLINE

#include <cmath>
#include <Nucleus/Fmt.h>
#include <sstream>

namespace Nucleus {

    template<typename T, typename... Args>
    String String::formatArgument(size_t n, String const& parameters, T const& element, Args&&... args) {

        if(n == 0) return Fmt<T>::format(element, parameters);

        return formatArgument(n-1, parameters, args...);

    }

    template<typename T>
    String String::formatArgument(size_t n, String const& parameters, T const& element)  {

        if(n != 0) nthrow("Too few arguments in pack or invalid argument index.");

        return Fmt<T>::format(element, parameters);

    }

    inline String String::format(String const& fmt) {
        return fmt;
    }
    
    template<typename... Args>
    String String::format(String const& fmt, Args&&... args) {

        String formatted(fmt.size());

        const size_t max = fmt.size();
        size_t last = 0;
        size_t index = 0;
        size_t arg = 0;

        while(index < max){

            if(fmt[index] == '{'){
                
                const size_t braceIndex = index;

                while(++index < max) {
                    
                    if(fmt[index] == '}') {
                        formatted.addMem(fmt.begin().get() + last, braceIndex - last);
                        break;
                    }
                    
                }

                if(index >= max) break;
                
                auto params = String(fmt.begin().get() + braceIndex + 1, index - braceIndex - 1);
                auto arr = params.split(", ");
                
                String& e = arr.size() > 0 ? arr[0] : params;

                if(Int64 o = 0; e.noThrowToInteger(o)){
                    formatted.addAll(formatArgument(o, arr.size() > 1 ? arr[1] : "", args...));
                }
                else{
                    formatted.addAll(formatArgument(arg++, params, args...));
                }
                
                last = index + 1;

            }

            ++index;

        }

        if(index != last){
            formatted.addMem(fmt.begin().get() + last, max - last);
        }

        return formatted;

    }

    template <typename T> requires std::is_integral_v<T>
    String String::fromInteger(T const& integral, size_t base) {
        
        if (integral == static_cast<T>(0)) return "0";

        Int64 n = i64(integral);

        // Static format buffer.
        static char buf[100];

        memset(buf, 0, sizeof buf);
        
        size_t offset = 0;

        if (integral < 0){
            n = std::abs(n);
            offset += 1;
            buf[0] = '-';
        }
        
        if(base == 16){
            buf[offset] = '0';
            buf[offset + 1] = 'x';
            offset += 2;
        }

        std::to_chars(buf + offset, buf + 100 - offset, n, static_cast<Int32>(base));
        
        return { buf, strlen(buf) };
            
    }

    template <typename T> requires std::is_floating_point_v<T>
    String String::fromFloatingPoint(T const& fp, Int64 precision) {

        std::stringstream stringstream;

        stringstream.setf(std::ios::fixed);

        if(precision > 0){
            stringstream.precision(precision);
        }

        stringstream << fp;
        
        return stringstream.str().c_str();
            
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
