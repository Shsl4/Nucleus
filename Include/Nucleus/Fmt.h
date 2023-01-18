#pragma once

#include <charconv>
#include <Nucleus/CoreMacros.h>

namespace Nucleus {

    template<typename Type>
    class Fmt;

    template <class T, std::size_t = sizeof(T)>
    std::true_type implements(T*);

    std::false_type implements(...);

    template <class T>
    using HasFmt = decltype(implements(std::declval<Fmt<T>*>()));
    
}

#define FMT_INLINE
#include <Nucleus/Inline/FmtImpl.inl>
#undef FMT_INLINE
