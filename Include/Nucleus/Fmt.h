#pragma once

#include <charconv>
#include <Nucleus/CoreMacros.h>
#include <Nucleus/String.h>

namespace Nucleus {
    
    template <bool Condition, class T = void>
    struct EnableIfImpl {};

    template <class T>
    struct EnableIfImpl<true, T> { using Type = T; };

    template <bool Condition, class T = void>
    using EnableIf = typename EnableIfImpl<Condition, T>::Type;

    template<typename Type, typename Restriction = Type>
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
