#pragma once

#include <charconv>
#include <functional>
#include <iostream>
#include <string>
#include <cstring>

#include <Nucleus/MutableArray.h>

namespace Nucleus {
    
    class String final : public Collection<char> {

        using Super = Collection<char>;

    public:

        String() = default;
        
        ~String() override;
        
        String(String const& other);
        
        String(std::string const& other);
        
        String(String&& other) noexcept;
        
        explicit String(size_t cap);

        String(const char* cString);

        String(const char* buf, size_t size);
        
        String& operator=(String const& other);
        
        String& operator=(String&& other) noexcept;

        String operator+(String const& other) const;

        String& operator+=(String const& other);

        auto add(const char &element) -> decltype(*this) & override;

        auto addAll(const Collection<char> &array) -> decltype(*this) & override;

        auto insert(const char &element, size_t index) -> decltype(*this) & override;

        auto insertAll(const Collection<char> &array, size_t index) -> decltype(*this) & override;

        bool removeAt(size_t index) override;

        bool removeAllOf(const char &element) override;

        bool operator==(String const& other) const;

        bool operator==(const char* other) const;

        String& insertAll(const char* string, size_t index);

        void clear() override;

        void removeOccurrences(String const& other);
        
        template<typename... Args>
        static String format(String const& fmt, Args&&... args);
        
        template<typename T>
        static String fromInteger(T const& integral);

        template<typename T>
        static String fromFloatingPoint(T const& fp);

        template<typename T>
        static String fromPointer(const T* pointer);

        NODISCARD Super::Iterator begin() const override;

        NODISCARD Super::Iterator end() const override;

        NODISCARD char& get(size_t index) const override;

        NODISCARD size_t size() const override { return count - 1; }

        NODISCARD bool contains(const char &element) const override;

        NODISCARD bool isEmpty() const override;

        NODISCARD MutableArray<String> split(String const& separators) const;

        NODISCARD MutableArray<String> split(char separator) const;

        NODISCARD String substring(size_t from, size_t to) const;

        NODISCARD size_t capacity() const override;

        NODISCARD Float64 toFloat64() const;

        NODISCARD Int64 toInteger() const;

        NODISCARD static bool isInteger(const char c) {
            return c >= L'0' && c <= L'9';
        }

        NODISCARD static UInt16 wcharToInteger(const char c) {
            return c - L'0';
        }


    private:

        friend String operator+(const char* cStr, String const& string);

        friend std::wostream& operator<<(std::wostream& os, String const& string);

        friend std::ostream& operator<<(std::ostream& os, String const& string);

        void extend(const size_t size) {

            if(count + size > storage){
                
                const size_t newSize = storage + size;
                Allocator<char>::reallocate(buffer, storage, newSize);
                memset(buffer + count, 0, newSize - count);
                storage = newSize;
                
            }

        }
        
        char* buffer = nullptr;
        size_t count = 0;
        size_t storage = 0;
        
    };
    
}

#define STRING_INLINE
#include <Nucleus/Inline/String.inl>
#undef STRING_INLINE
