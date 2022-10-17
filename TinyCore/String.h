#pragma once

#include "MutableArray.h"
#include <iostream>

namespace TinyCore {
    
    class String : public MutableArray<wchar_t> {

    public:

        String() = default;

        String(const char* cString);

        String(const char* buf, size_t size);

        String(const wchar_t* wString);

        String(const wchar_t* buf, size_t size);

        bool operator==(String const& other) const;

        FORCEINLINE size_t getSize() const { return count - 1; }

        FORCEINLINE size_t getCount() const override { return count - 1; }
        
        MutableArray<String> split(String const& separators) const;
        
        MutableArray<String> split(char separator) const;
        
        MutableArray<String> split(wchar_t separator) const;

        bool contains(char c) const;

        bool contains(wchar_t wchar) const;

        void removeOccurrences(String const& other);

        friend std::wostream& operator<<(std::wostream& os, String const& string);

        friend std::ostream& operator<<(std::ostream& os, String const& string);

    };
    
}

#define STRING_INLINE
#include "Inline/String.inl"
#undef STRING_INLINE
