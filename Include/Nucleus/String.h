#pragma once

#include <charconv>
#include <functional>
#include <Nucleus/CoreMacros.h>
#include <Nucleus/Allocator.h>
#include <Nucleus/MutableArray.h>
#include <iostream>
#include <string>

namespace Nucleus {
    
    class String final {

    public:

        String() = default;
        
        ~String();

        String(String const& other);
        
        String(String&& other) noexcept;
        
        explicit String(size_t cap);

        String(const char* cString);

        String(const char* buf, size_t size);

        String(const wchar_t* wString);

        String(const wchar_t* buf, size_t size);
        
        String& operator=(String const& other);
        
        String& operator=(String&& other) noexcept;

        String operator+(String const& other) const;

        String& operator+=(String const& other);
        
        bool operator==(String const& other) const;

        bool operator==(const char* other) const;

        String& append(String const& other);
        
        NODISCARD MutableArray<String> split(String const& separators) const;
        
        NODISCARD MutableArray<String> split(char separator) const;
        
        NODISCARD MutableArray<String> split(wchar_t separator) const;

        NODISCARD String substring(size_t from, size_t to) const;

        NODISCARD bool contains(char c) const;

        NODISCARD bool contains(wchar_t wchar) const;

        void clear();

        void removeOccurrences(String const& other);

        NODISCARD wchar_t* begin() const { return buffer; }
        
        NODISCARD wchar_t* end() const { return buffer + count; }
        
        template<typename... Args>
        static String format(String const& fmt, Args&&... args);
        
        NODISCARD Float64 toFloat64() const;
        
        NODISCARD Int64 toInteger() const;
        
        template<typename T>
        static String fromInteger(T const& integral);

        template<typename T>
        static String fromFloatingPoint(T const& fp);

        template<typename T>
        static String fromPointer(const T* pointer);

        friend std::wostream& operator<<(std::wostream& os, String const& string);

        friend std::ostream& operator<<(std::ostream& os, String const& string);
        
        FORCEINLINE size_t getSize() const { return count - 1; }

        FORCEINLINE static bool isInteger(const wchar_t c) {
            return c >= L'0' && c <= L'9';
        }
        
        FORCEINLINE static UInt8 wcharToInteger(const wchar_t c) {
            return c - L'0';
        }
        
    private:

        void reserve(const size_t size) {

            if(count + size > capacity){
                
                const size_t newSize = capacity + size;
                Allocator<wchar_t>::reallocate(buffer, capacity, newSize);
                memset(buffer + count, 0, newSize - count);
                capacity = newSize;
                
            }

        }
        
        wchar_t* buffer = nullptr;
        size_t count = 0;
        size_t capacity = 0;
        
    };
    
}

#define STRING_INLINE
#include "Inline/String.inl"
#undef STRING_INLINE
