#pragma once

#ifdef STRING_INLINE

#include <Nucleus/Shared.h>
#include "Nucleus/String.h"

namespace Nucleus {

    inline void ensure(bool condition, class String const& str){

        if(!condition){
            throw Exceptions::Exception(str.begin().get());
        }

    }

    inline void ensure(bool condition){

        if(!condition){
            throw Exceptions::Exception("Unhandled exception.");
        }

    }
    
    inline String::String(String const& other){
        *this = other;
    }
    
    inline String::String(String&& other) noexcept {
        *this = std::move(other);
    }

    inline String::String(size_t cap) {

        extend(cap);
        
    }
    
    inline String::String(const char* cString) {

        if (!cString) { return; }

        const size_t stringSize = strlen(cString) + 1;
        this->buffer = Allocator<char>::allocate(stringSize);

        Allocator<char>::copy(cString, cString + stringSize, buffer);

        this->count = this->storage = stringSize;

        this->buffer[stringSize - 1] = L'\0';

    }
    
    inline String::String(std::string const& string) {

        const size_t stringSize = string.size() + 1;
        this->buffer = Allocator<char>::allocate(stringSize);

        Allocator<char>::copy(string.data(), string.data() + stringSize, buffer);

        this->count = this->storage = stringSize;

        this->buffer[stringSize - 1] = L'\0';

    }
    
    inline String::~String(){
        
        Allocator<char>::release(buffer);
        this->count = 0;
        this->storage = 0;
        
    }

    inline String::String(const char* buf, size_t size) {

        if (!buf) { return; }

        const size_t stringSize = size + 1;

        this->buffer = Allocator<char>::allocate(stringSize);

        Allocator<char>::copy(buf, buf + stringSize, buffer);

        this->count = this->storage = stringSize;
        
        this->buffer[stringSize - 1] = L'\0';

    }

    
    inline String& String::operator=(String const& other){
        
        if(&other == this ) return *this;
        
        const char* start = other.buffer;

        if (!other.buffer) return *this;

        Allocator<char>::reallocate(buffer, storage, other.storage);
        Allocator<char>::copy(start, start + other.storage, buffer);

        this->count = other.count;
        this->storage = other.storage;
            
        return *this;
        
    }
    
    inline String& String::operator=(String&& other) noexcept{
        
        if(&other == this) return *this;

        Allocator<char>::release(buffer);

        this->buffer = other.buffer;
        this->count = other.count;
        this->storage = other.storage;

        other.buffer = nullptr;
        other.count = 0;
        other.storage = 0;
            
        return *this;
        
    }

    inline bool String::operator==(String const& other) const {

        if (size() != other.size()) return false;

        for (size_t i = 0; i < size(); ++i) {

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
        e.addAll(other);
        return e;
    }
    
    inline String& String::operator+=(String const& other) {
        return addAll(other);
    }

    inline void String::clear() {
        memset(buffer, 0, storage);
        this->count = 0;
    }

    inline MutableArray<String> String::split(const char separator) const {
        
        MutableArray<String> components;

        const size_t sz = this->size();
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
            
        const size_t size = this->size();
        const size_t otherSize = other.size();

        size_t i = 0, j = 0;
            
        while(i != size) {

            if(buffer[i] == other.buffer[j]) {
                   
                if(++j == otherSize) {

                    Allocator<char>::move(buffer + i + 1, buffer + size, buffer + i + 1 - otherSize);
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

        const size_t sz = this->size();
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
        return os << string.buffer;
    }

    String operator+(const char *cStr, String const &string) {
        String newString = cStr;
        return newString.addAll(string);
    }

    String::Super::Iterator String::begin() const {
        return Iterator(buffer);
    }

    String::Super::Iterator String::end() const {
        return Iterator(buffer + count - 1);
    }

    char &String::get(size_t index) const {
        return buffer[index];
    }

    auto String::add(const char &element) -> decltype(*this)& {

        if (!element) return *this;

        extend(1);
        buffer[count++] = element;
        return *this;

    }

    auto String::addAll(const Collection<char> &array) -> decltype(*this)& {

        extend(array.size());

        Allocator<char>::copy(array.begin().get(), array.end().get(), buffer + size());

        this->count += array.size();

        return *this;

    }

    auto String::insert(const char &element, size_t index) -> decltype(*this)& {

        if (!element) return *this;

        if (index >= count) {
            return add(element);
        }

        extend(1);

        Allocator<char>::move(buffer + index, buffer + count, buffer + index + 1);

        buffer[index] = element;
        ++count;

        return *this;

    }

    auto String::insertAll(const Collection<char> &array, size_t index) -> decltype(*this)& {

        return insertAll(array.begin().get(), index);

    }


    String& String::insertAll(const char *string, size_t index) {

        const size_t stringSize = strlen(string);

        if (stringSize == 0) return *this;

        extend(stringSize);

        Allocator<char>::move(buffer + index, buffer + count, buffer + index + stringSize);

        for(size_t i = 0; i < stringSize; ++i){

            buffer[index++] = string[i];
            ++count;

        }

        return *this;

    }

    bool String::removeAt(size_t index) {

        if (index > size()) return false;

        Allocator<char>::move(buffer + index + 1, buffer + count, buffer + index);

        --count;

        return true;

    }

    bool String::removeAllOf(const char &element) {

        bool r = false;

        for(size_t i = 0; i < size(); ++i){

            if (buffer[i] == element){
                removeAt(i--);
                r = true;
            }

        }

        return r;

    }

    bool String::contains(const char &element) const {

        for(auto& c : *this) {
            if(c == element) return true;
        }

        return false;

    }

    bool String::isEmpty() const { return count == 0; }

    size_t String::capacity() const { return storage; }

}

#include <Nucleus/Inline/StringFormat.inl>
#endif
