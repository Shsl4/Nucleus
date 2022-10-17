#pragma once

#ifdef STRING_INLINE

namespace TinyCore {

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

    inline bool String::operator==(String const& other) const {

        if (getCount() != other.getCount()) return false;

        for (size_t i = 0; i < getCount(); ++i) {

            if (buffer[i] != other.buffer[i]) {
                return false;
            }

        }

        return true;

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

#endif
