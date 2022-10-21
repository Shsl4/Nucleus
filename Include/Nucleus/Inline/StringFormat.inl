#pragma once

#ifdef STRING_INLINE

#include <cmath>
#include <Nucleus/Fmt.h>

namespace Nucleus {
    
    inline Float64 String::toFloat64() const {
        
        if(getSize() == 0) throw Exceptions::ParseError("Cannot parse an empty string");

        const auto max = static_cast<Int64>(getSize());
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
            
            const UInt8 digit = wcharToInteger(buffer[i]);
            
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
        
        if(getSize() == 0) throw Exceptions::ParseError("Cannot parse an empty string");

        const size_t max = getSize();
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
    
    template<typename T, typename... Args>
    EnableIf<HasFmt<T>::value, void>
    expand(MutableArray<String>& storage, T const& element, Args&& ... args) {
                
        storage += Fmt<T>::format(element);
        
        if (sizeof...(args) > 0) expand(storage, args...);
        
    }
    
    template<typename T, typename... Args>
    EnableIf<HasFmt<T>::value, void>
    expand(MutableArray<String>& storage, T const& element) {
        
        storage += Fmt<T>::format(element);
        
    }
    
    inline void expand(MutableArray<String>&) { }
    
    template<typename... Args>
    String String::format(String const& fmt, Args&&... args) {

        MutableArray<String> arguments(sizeof...(args));
        
        // If no matching function is found, T must implement a formatter.
        expand(arguments, args...);
        
        if(arguments.getCount() == 0) return fmt;

        String formatted(fmt.getSize());

        size_t nextArg = 0;
        size_t last = 0;
        bool scan = false;

        for (size_t i = 0; i < fmt.getSize(); ++i) {

            if(!scan && fmt.buffer[i] == '{') {

                formatted.reserve(i - last);
                Allocator<wchar_t>::copy(fmt.buffer + last, fmt.buffer + i, formatted.buffer + formatted.count);
                formatted.count += i - last;
                scan = true;
                last = i + 1;

            }

            if(scan && fmt.buffer[i] == '}') {

                if(last == i) {
                    
                    if (nextArg >= arguments.getCount()){
                        throw Exceptions::BadFormat("Uneven formatting braces / Argument count.");
                    }

                    if(String const& r = arguments[nextArg]; r.count > 1){
                        
                        formatted.reserve(r.count);
                        Allocator<wchar_t>::copy(r.buffer, r.buffer + r.count, formatted.buffer + formatted.count);
                        formatted.count += r.getSize();
                        
                    }
                    
                    ++nextArg;
                    
                }
                else {
                    
                    try {
                        
                        const size_t index = fmt.substring(last, i).toInteger();
                        
                        if(index >= arguments.getCount()){
                            throw Exceptions::BadFormat("Argument index out of range.");
                        }

                        String const& r = arguments[index];

                        if(r.count > 1){
                        
                            formatted.reserve(r.count);
                            Allocator<wchar_t>::copy(r.buffer, r.buffer + r.count, formatted.buffer + formatted.count);
                            formatted.count += r.getSize();
                        
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

        if (last != fmt.getSize()) {
            formatted.reserve(fmt.count - last);
            Allocator<wchar_t>::copy(fmt.buffer + last, fmt.buffer + fmt.count, formatted.buffer + formatted.count);
            formatted.count += fmt.getSize() - last;
        }

        // Add 1 to count the null byte.
        formatted.count += 1;
        
        return formatted;
        
    }

    template <typename T>
    String String::fromInteger(T const& integral) {
            
        static_assert(std::is_integral_v<T>, "T must be an integral type");

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

        return std::to_wstring(fp).c_str();
            
    }

    template <typename T>
    String String::fromPointer(const T* pointer) {

        if(!pointer) { return format("nullptr ({}*)", Type::getClassName<T>()); }
            
        const auto address = reinterpret_cast<intptr_t>(pointer);
            
        // Static format buffer.
        static char buf[100];
            
        std::to_chars(buf, buf + 100, address, 16);
            
        const String strAddress(buf, strlen(buf));
            
        return format("0x{} ({}*)", strAddress, Type::getClassName<T>());
            
    }

}

#endif
