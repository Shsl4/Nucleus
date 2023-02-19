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
        static String name(T const& object);

        template<typename T>
        static String name();

        static String name(std::type_info const& type);

        template<typename T>
        static std::type_info const& of(T const& object);

        template<typename T>
        static std::type_info const& of();

#ifdef __GNUG__

    private:

        static class String demangle(const char* name);

#endif

    };

}

#define TYPE_INLINE
#include <Nucleus/Inline/Type.inl>
#undef TYPE_INLINE
