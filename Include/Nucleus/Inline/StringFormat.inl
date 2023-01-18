#pragma once

#ifdef STRING_INLINE

#include <cmath>
#include <Nucleus/Fmt.h>

namespace Nucleus {
    
    inline Float64 String::toFloat64() const {
        
        if(size() == 0) throw Exceptions::ParseError("Cannot parse an empty string");

        const auto max = static_cast<Int64>(size());
        double value = 0;
        const bool negative = buffer[0] == '-';

        Int64 dotIndex = -1;
        Int64 counter = 0;
        
        for(const auto& e : *this){
            
            if (e == '.' || e == ',') {
                dotIndex = counter;
                break;
            }
            
            ++counter;
            
        }

        if(dotIndex == -1) { return static_cast<Float64>(toInteger()); }

        for(Int64 i = negative; i < max; ++i){
            
            if(dotIndex == i) { continue; }
            
            if(!isInteger(buffer[i])) {
                throw Exceptions::ParseError("Failed to parse: String does not represent a number.");
            }
            
            const UInt16 digit = wcharToInteger(buffer[i]);
            
            if(i < dotIndex){
                value += digit * std::pow(10, dotIndex - i - 1);
            }
            else{
                const auto v = dotIndex - i;
                value += digit * std::pow(10, v);
            }
            
        }
        
        if (negative) { value = -value; }
        
        return value;
        
    }
    
    inline Int64 String::toInteger() const {
        
        if(size() == 0) throw Exceptions::ParseError("Cannot parse an empty string");

        const size_t max = size();
        const bool negative = buffer[0] == '-';
        Int64 value = 0;

        for(size_t i = negative; i < max; ++i){

            if(!isInteger(buffer[i])) {
                throw Exceptions::ParseError("Failed to parse: String does not represent an integer.");
            }
                        
            value += static_cast<Int64>(wcharToInteger(buffer[i]) * std::pow(10, count - i - 2));
            
        }
        
        if (negative) {
            value = -value;
        }
        
        return value;
        
    }
    
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

        String formatted(fmt.size());

        size_t nextArg = 0;
        size_t last = 0;
        bool scan = false;

        for (size_t i = 0; i < fmt.size(); ++i) {

            if(!scan && fmt.buffer[i] == '{') {

                formatted.extend(i - last);
                Allocator<char>::copy(fmt.buffer + last, fmt.buffer + i, formatted.buffer + formatted.count);
                formatted.count += i - last;
                scan = true;
                last = i + 1;

            }

            if(scan && fmt.buffer[i] == '}') {

                if(last == i) {
                    
                    if (nextArg >= arguments.size()){
                        throw Exceptions::BadFormat("Uneven formatting braces / Argument count.");
                    }

                    if(String const& r = arguments[nextArg]; r.count > 1){

                        formatted.extend(r.count);
                        Allocator<char>::copy(r.buffer, r.buffer + r.count, formatted.buffer + formatted.count);
                        formatted.count += r.size();
                        
                    }
                    
                    ++nextArg;
                    
                }
                else {
                    
                    try {
                        
                        const size_t index = fmt.substring(last, i).toInteger();
                        
                        if(index >= arguments.size()){
                            throw Exceptions::BadFormat("Argument index out of range.");
                        }

                        String const& r = arguments[index];

                        if(r.count > 1){

                            formatted.extend(r.count);
                            Allocator<char>::copy(r.buffer, r.buffer + r.count, formatted.buffer + formatted.count);
                            formatted.count += r.size();
                        
                        }
     
                    }
                    catch(Exceptions::ParseError const&){
            
                        throw Exceptions::BadFormat("Unexpected content found in format braces.");
            
                    }
                    
                }

                last = i + 1;
                scan = false;
                
            }
            
        }

        if (last != fmt.size()) {
            formatted.extend(fmt.count - last);
            Allocator<char>::copy(fmt.buffer + last, fmt.buffer + fmt.count, formatted.buffer + formatted.count);
            formatted.count += fmt.size() - last;
        }

        // Add 1 to count the null byte.
        formatted.count += 1;
        
        return formatted;
        
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
