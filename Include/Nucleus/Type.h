#pragma once

#include <typeinfo>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace Nucleus {

    class Type {

    public:

        template<typename T>
        static class String name(T const& object);

        template<typename T>
        static class String name();

        static class String name(std::type_info const& type);

        template<typename T>
        static std::type_info const& of(T const& object);

        template<typename T>
        static std::type_info const& of();

#ifdef __GNUG__

    private:

        inline static class String demangle(const char* name);

#endif

    };

}

#define TYPE_INLINE
#include <Nucleus/Inline/Type.inl>
#undef TYPE_INLINE
