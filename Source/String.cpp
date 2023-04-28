#include <Nucleus/String.h>
#include <Nucleus/Exceptions.h>

namespace Nucleus{

    String::String(String const& other){
        *this = other;
    }

    String::String(String&& other) noexcept {
        *this = std::move(other);
    }

    String::String(size_t cap) {
        extend(cap);
    }

    String::String(size_t cap, char data) {
        
        extend(cap + 1);

        for(size_t i = 0; i < cap; ++i) {
            buffer[i] = data;
        }
        
        this->count = this->capacity();
        
    }

    String::String(const char* cString) {

        if (!cString) { return; }

        const size_t stringSize = strlen(cString) + 1;
        this->buffer = Allocator<char>::allocate(stringSize);

        Allocator<char>::copy(cString, cString + stringSize, buffer);

        this->count = this->storage = stringSize;

        this->buffer[stringSize - 1] = '\0';

    }

    String::String(std::string const& string) {

        const size_t stringSize = string.size() + 1;
        this->buffer = Allocator<char>::allocate(stringSize);

        Allocator<char>::copy(string.data(), string.data() + stringSize, buffer);

        this->count = this->storage = stringSize;

        this->buffer[stringSize - 1] = L'\0';

    }

    String::~String(){

        Allocator<char>::release(buffer);
        this->count = 0;
        this->storage = 0;

    }

    String::String(const char* buf, size_t size) {

        if (!buf) { return; }

        const size_t stringSize = size + 1;

        this->buffer = Allocator<char>::allocate(stringSize);

        Allocator<char>::copy(buf, buf + stringSize, buffer);

        this->count = this->storage = stringSize;

        this->buffer[stringSize - 1] = L'\0';

    }


    String& String::operator=(String const& other){

        if(&other == this) return *this;

        const char* start = other.buffer;

        if (!other.buffer) return *this;

        Allocator<char>::reallocate(buffer, storage, other.storage);
        Allocator<char>::copy(start, start + other.storage, buffer);

        this->count = other.count;
        this->storage = other.storage;

        return *this;

    }

    String& String::operator=(String&& other) noexcept{

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

    bool String::operator==(String const& other) const {

        if (size() != other.size()) return false;

        for (size_t i = 0; i < size(); ++i) {

            if (buffer[i] != other.buffer[i]) {
                return false;
            }

        }

        return true;

    }

    bool String::operator==(const char* other) const {

        if(strlen(other) != count - 1) return false;

        for (size_t i = 0; i < count - 1; ++i) {

            if(buffer[i] != other[i]) return false;

        }

        return true;

    }

    String String::operator+(String const& other) const {
        auto e = String(*this);
        e.addAll(other);
        return e;
    }

    String& String::operator+=(String const& other) {
        return addAll(other);
    }

    void String::clear() {
        memset(buffer, 0, storage);
        this->count = 0;
    }

    size_t String::firstIndexOf(const char c, const size_t from) const {

        for (size_t i = from; i < size(); ++i) {
            
            if (buffer[i] == c) {
                return i;
            }
            
        }

        throw Exceptions::LightException("The string does not contain the specified character");
        
    }

    size_t String::lastIndexOf(const char c) const {

        for (size_t i = count; i > 0; --i) {
            
            if (buffer[i - 1] == c) {
                return i - 1;
            }
            
        }

        throw Exceptions::LightException("The string does not contain the specified character");
        
    }

    size_t String::numberOccurrences(const char c) const {

        size_t ct = 0;

        for (size_t i = 0; i < count; ++i) { if(buffer[i] == c) ++ct; }

        return ct;
        
    }

    MutableArray<String> String::split(const char separator) const {

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


    String String::substring(const size_t from, const size_t to) const {

        if(from > count && to > count) throw Exceptions::OutOfRange("Substring index out of range.");
        if(from >= to) return "";

        return { buffer + from, to - from };

    }

    String& String::removeOccurrences(String const& other) {

        size_t size = this->size();
        const size_t otherSize = other.size();

        size_t i = 0, j = 0;

        while(i < size) {

            if(buffer[i] == other.buffer[j]) {

                if(++j == otherSize) {
                    
                    const auto from = buffer + i + 1;
                    const auto to = buffer + count;
                    Allocator<char>::move(from, to, from - j);
                    memset(to - j, '\0', j);
                    this->count -= j;
                    i -= j;
                    j = 0;

                }

            }
            else {

                i -= j;
                j = 0;

            }

            ++i;

        }

        return *this;

    }

    String &String::replaceOccurrences(const String &of, const String &with) {

        size_t size = this->size();
        const size_t ofSize = of.size();

        size_t i = 0, j = 0;

        while(i < size) {

            if(buffer[i] == of.buffer[j]) {

                if(++j == ofSize) {

                    const auto from = buffer + i + 1;
                    const auto to = buffer + count;
                    Allocator<char>::move(from, to, from - j);
                    memset(to - j, '\0', j);
                    this->count -= j;
                    i -= j;
                    j = 0;
                    insertAll(with, i + 1);
                    i += with.size();

                }

            }
            else {

                i -= j;
                j = 0;

            }

            ++i;

        }

        return *this;

    }

    
    MutableArray<String> String::split(String const& separators) const {

        MutableArray<String> components;

        const size_t sz = this->size();
        size_t from = 0;

        for (size_t i = 0; i < sz; ++i) {

            if (separators.contains(buffer[i])){
                
                if (const size_t size = i - from; size != 0) {

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

    std::wostream& operator<<(std::wostream& os, String const& string) {
        return os << string.buffer;
    }

    std::ostream& operator<<(std::ostream& os, String const& string) {
        return os << string.buffer;
    }

    String operator+(const char *cStr, String const &string) {
        String newString = cStr;
        return newString.addAll(string);
    }

    String::Super::Iterator String::begin() const {
        return Iterator(Allocator<ContinuousIterator>::construct(buffer));
    }

    String::Super::Iterator String::end() const {
        return Iterator(Allocator<ContinuousIterator>::construct(buffer + count - 1));
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

    auto String::addAll(const Collection &collection) -> decltype(*this)& {

        if (collection.isEmpty()) return *this;

        extend(collection.size() + 1);

        for(char const& element : collection){
            buffer[count++ - 1] = element;
        }

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

    auto String::insertAll(const Collection<char> &collection, size_t index) -> decltype(*this)& {

        return insertAll(collection.begin().get(), index);

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

    bool String::remove(const char &element) {

        bool r = false;

        for(size_t i = 0; i < size(); ++i){

            if (buffer[i] == element){
                removeAt(i--);
                r = true;
            }

        }

        return r;

    }


    auto String::removeAll(const Collection &collection) -> decltype(*this)& {

        for(auto const& e : collection){
            remove(e);
        }

        return *this;
    }

    bool String::contains(const char &element) const {

        for(const auto& c : *this) {
            if(c == element) return true;
        }

        return false;

    }

    bool String::isEmpty() const { return count == 0 || buffer[0] == '\0'; }

    size_t String::capacity() const { return storage; }

    Float64 String::toFloat64() const {

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

            const UInt16 digit = charToInteger(buffer[i]);

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

    bool String::toBool() const {

        if(*this == "0") return false;
        if(*this == "1") return true;
        
        const String copy = toLower();
        
        if (copy == "true") return true;
        if (copy == "false") return false;

        throw Exceptions::ParseError("This string does not represent a bool.");
        
    }

    String String::toLower() const {

        String copy = *this;

        for (auto& e : copy) {
            e = tolower(e);
        }

        return copy;
        
    }

    bool String::noThrowToInteger(Int64& out) const noexcept {

        if(size() == 0) return false;

        const size_t max = size();
        const bool negative = buffer[0] == '-';
        Int64 value = 0;

        for(size_t i = negative; i < max; ++i){

            if(!isInteger(buffer[i])) { return false; }

            value += static_cast<Int64>(charToInteger(buffer[i]) * std::pow(10, count - i - 2));

        }

        if (negative) { value = -value; }

        out = value;
        
        return true;

    }

    Int64 String::toInteger() const {

        if(size() == 0) throw Exceptions::ParseError("Cannot parse an empty string");

        const size_t max = size();
        const bool negative = buffer[0] == '-';
        Int64 value = 0;

        for(size_t i = negative; i < max; ++i){

            if(!isInteger(buffer[i])) {
                throw Exceptions::ParseError("Failed to parse: String does not represent a number.");
            }

            value += static_cast<Int64>(charToInteger(buffer[i]) * std::pow(10, count - i - 2));

        }

        if (negative) { value = -value; }
        
        return value;
        
    }


    void String::extend(const size_t size) {

        if(count + size > storage){

            const size_t newSize = storage + size;
            Allocator<char>::reallocate(buffer, storage, newSize);
            memset(buffer + count, 0, newSize - count);
            storage = newSize;
            
            if(count == 0){
                buffer[0] = '\0';
                ++count;
            }

        }

    }

    String &String::addMem(const char *mem, size_t size) {

        if(size == 0) return *this;

        extend(size);

        for(size_t i = 0; i < size; ++i){
            buffer[count++ - 1] = mem[i];
        }

        return *this;

    }

}
