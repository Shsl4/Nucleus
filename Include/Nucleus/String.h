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

        String(size_t cap, char data);

        String(const char* cString);

        String(const unsigned char* cString);

        String(const char* buf, size_t size);
        
        String& operator=(String const& other);
        
        String& operator=(String&& other) noexcept;

        String operator+(String const& other) const;

        String& operator+=(String const& other);
        
        bool operator==(String const& other) const;

        bool operator==(const char* other) const;

        auto add(const char &element) -> decltype(*this) & override;

        auto addAll(const Collection &collection) -> decltype(*this) & override;

        auto insert(const char &element, size_t index) -> decltype(*this) & override;

        auto insertAll(const Collection &collection, size_t index) -> decltype(*this) & override;

        bool removeAt(size_t index) override;

        bool remove(const char &element) override;

        auto removeAll(const Collection<char> &collection) -> decltype(*this) & override;

        String& addMem(const char* mem, size_t size);

        String& insertAll(const char* string, size_t index);
        
        String& removeOccurrences(String const& other);

        void clear() override;
        
        template<typename... Args>
        static String format(String const& fmt, Args&&... args);
        
        template<typename T> requires std::is_integral_v<T>
        static String fromInteger(T const& integral, size_t base = 10);

        template<typename T> requires std::is_floating_point_v<T>
        static String fromFloatingPoint(T const& fp);

        template<typename T>
        static String fromPointer(const T* pointer);

        NODISCARD Iterator begin() const override;

        NODISCARD Iterator end() const override;

        NODISCARD char& get(size_t index) const override;

        NODISCARD size_t size() const override { return count - 1; }

        NODISCARD bool contains(const char &element) const override;

        NODISCARD bool isEmpty() const override;

        NODISCARD MutableArray<String> split(String const& separators) const;

        NODISCARD MutableArray<String> split(char separator) const;

        NODISCARD String substring(size_t from, size_t to) const;

        NODISCARD size_t firstIndexOf(char c, size_t from = 0) const;

        NODISCARD size_t lastIndexOf(char c) const;

        NODISCARD size_t numberOccurrences(char c) const;

        NODISCARD size_t capacity() const override;

        NODISCARD Float64 toFloat64() const;

        NODISCARD bool toBool() const;

        NODISCARD String toLower() const;
        
        bool noThrowToInteger(Int64& out) const noexcept;
        
        bool toInteger(Int64& out) const;

        NODISCARD static bool isInteger(const char c) {
            return c >= L'0' && c <= L'9';
        }

        NODISCARD static UInt16 charToInteger(const char c) {
            return c - '0';
        }


    private:

        friend String operator+(const char* cStr, String const& string);

        friend std::wostream& operator<<(std::wostream& os, String const& string);

        friend std::ostream& operator<<(std::ostream& os, String const& string);
        
        template<typename T, typename... Args>
        static String formatArgument(size_t n, String const& parameters, T const& element, Args&&... args);

        template<typename T>
        static String formatArgument(size_t n, String const& parameters, T const& element);
        
        void extend(size_t size);
                
        char* buffer = nullptr;
        size_t count = 0;
        size_t storage = 0;

    };
    
}

#define STRING_INLINE
#include <Nucleus/Inline/StringFormat.inl>
#undef STRING_INLINE
