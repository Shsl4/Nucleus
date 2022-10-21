#pragma once

#ifdef STRING_INLINE

#include <Nucleus/Shared.h>
#include <sstream>

namespace Nucleus {
    
    inline String::String(String const& other){
        *this = other;
    }
    
    inline String::String(String&& other) noexcept {
        *this = std::move(other);
    }

    inline String::String(size_t cap) {

        reserve(cap);
        
    }

    inline String::String(const char* cString) {

        if (!cString) { return; }

        const size_t stringSize = strlen(cString) + 1;
        this->buffer = Allocator<wchar_t>::allocate(stringSize);

#ifdef _WIN32
        mbstowcs_s(nullptr, buffer, stringSize, cString, stringSize);
#else
        mbstowcs(buffer, cString, stringSize);
#endif

        this->count = this->capacity = stringSize;

        this->buffer[stringSize - 1] = L'\0';

    }
    
    inline String::~String(){
        
        Allocator<wchar_t>::release(buffer);
        this->count = 0;
        this->capacity = 0;
        
    }

    inline String::String(const wchar_t* wString) {

        if (!wString) { return; }

        const size_t stringSize = wcslen(wString) + 1;
        this->buffer = Allocator<wchar_t>::allocate(stringSize);

#ifdef _WIN32
        wcscpy_s(buffer, stringSize, wString);
#else
        wcscpy(buffer, wString);
#endif

        this->count = this->capacity = stringSize;

        this->buffer[stringSize - 1] = L'\0';

    }

    inline String::String(const char* buf, size_t size) {

        if (!buf) { return; }

        const size_t stringSize = size + 1;

        this->buffer = Allocator<wchar_t>::allocate(stringSize);

#ifdef _WIN32
        mbstowcs_s(nullptr, buffer, stringSize, buf, stringSize);
#else
        mbstowcs(buffer, buf, stringSize);
#endif

        this->count = this->capacity = stringSize;
        
        this->buffer[stringSize - 1] = L'\0';

    }

    inline String::String(const wchar_t* buf, size_t size) {

        if (!buf) { return; }

        const size_t stringSize = size + 1;
        this->buffer = Allocator<wchar_t>::allocate(stringSize);

        Allocator<wchar_t>::copy(buf, buf + size, buffer);

        this->buffer[size] = 0;

        this->count = this->capacity = stringSize;

    }
    
    inline String& String::operator=(String const& other){
        
        if(&other == this) return *this;
        
        const wchar_t* start = other.buffer;

        Allocator<wchar_t>::reallocate(buffer, capacity, other.capacity);
        Allocator<wchar_t>::copy(start, start + other.capacity, buffer);

        this->count = other.count;
        this->capacity = other.capacity;
            
        return *this;
        
    }
    
    inline String& String::operator=(String&& other) noexcept{
        
        if(&other == this) return *this;

        Allocator<wchar_t>::release(buffer);

        this->buffer = other.buffer;
        this->count = other.count;
        this->capacity = other.capacity;

        other.buffer = nullptr;
        other.count = 0;
        other.capacity = 0;
            
        return *this;
            
        
    }

    inline bool String::operator==(String const& other) const {

        if (getSize() != other.getSize()) return false;

        for (size_t i = 0; i < getSize(); ++i) {

            if (buffer[i] != other.buffer[i]) {
                return false;
            }

        }

        return true;

    }

    inline bool String::operator==(const char* other) const {

        if(strlen(other) != count - 1) return false;

        for (size_t i = 0; i < count - 1; ++i) {
            
            if(buffer[i] != static_cast<wchar_t>(other[i])) return false;
                
        }
        
        return true;
        
    }

    inline String String::operator+(String const& other) const {
        auto e = String(*this);
        e.append(other);
        return e;
        
    }
    
    inline String& String::operator+=(String const& other) {
        
        return append(other);
        
    }

    inline String& String::append(String const& other) {

        reserve(other.getSize());

        Allocator<wchar_t>::copy(other.buffer, other.buffer + other.count, buffer + getSize());

        this->count += other.getSize();

        memset(buffer + count, 0, capacity - count);
        
        return *this;
        
    }

    inline bool String::contains(const char c) const {
        return contains(static_cast<wchar_t>(c));
    }

    inline bool String::contains(const wchar_t wchar) const {
            
        for (auto const& c : *this) {
                
            if(c == wchar) return true;
                
        }
            
        return false;
            
    }

    inline void String::clear() {
        memset(buffer, 0, capacity);
        this->count = 0;
    }

    inline MutableArray<String> String::split(const char separator) const {
        return split(static_cast<wchar_t>(separator));
    }

    inline MutableArray<String> String::split(const wchar_t separator) const {

        MutableArray<String> components;

        const size_t sz = this->getSize();
        size_t from = 0;

        for (size_t i = 0; i < sz; ++i) {

            if (buffer[i] == separator) {

                const size_t size = i - from;

                if (size != 0) {

                    components += String(buffer + from, size);

                }

                from = i + 1;

            }

        }

        if (from != sz) {

            components += String(buffer + from, sz - from);

        }

        return components;

    }

    inline String String::substring(const size_t from, const size_t to) const {

        if(from > count && to > count) throw Exceptions::OutOfRange("Substring index out of range.");
        if(from >= to) return "";

        return { buffer + from, to - from };
        
    }

    inline void String::removeOccurrences(String const& other) {
            
        const size_t size = getSize();
        const size_t otherSize = other.getSize();

        size_t i = 0, j = 0;
            
        while(i != size) {

            if(buffer[i] == other.buffer[j]) {
                   
                if(++j == otherSize) {

                    Allocator<wchar_t>::move(buffer + i + 1, buffer + size, buffer + i + 1 - otherSize);
                    memset(buffer + size - otherSize, 0, otherSize);
                    this->count -= otherSize;
                    i = j = 0;
                    continue;
                        
                }
                    
            }
            else {
                    
                i -= j;
                j = 0;
                    
            }

            ++i;
                
        }
            
    }

    inline MutableArray<String> String::split(String const& separators) const {

        MutableArray<String> components;

        const size_t sz = this->getSize();
        size_t from = 0;

        for (size_t i = 0; i < sz; ++i) {

            if (separators.contains(buffer[i])){

                const size_t size = i - from;

                if (size != 0) {

                    components += String(buffer + from, size);

                }

                from = i + 1;

            }

        }

        if (from != sz) {

            components += String(buffer + from, sz - from);

        }

        return components;
        
    }

    inline std::wostream& operator<<(std::wostream& os, String const& string) {
        return os << string.buffer;
    }

    inline std::ostream& operator<<(std::ostream& os, String const& string) {

        for (const auto& wc : string) {

            if(wc == 0) break;
            os << static_cast<char>(wc);

        }

        return os;
    }
    
}

#include "StringFormat.inl"

#endif
