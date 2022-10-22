#pragma once

#include <typeinfo>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace Nucleus {

    class String;

    class Type {

    public:

        template<typename T>
        static String getClassName(T const& object);

        template<typename T>
        static String getClassName();

#ifdef __GNUG__

    private:

        inline static String demangle(const char* name);

#endif

    };

}

#define TYPE_INLINE
#include "Inline/Type.inl"
#undef TYPE_INLINE
