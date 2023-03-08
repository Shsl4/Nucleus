#pragma once

#include <charconv>
#include <Nucleus/CoreMacros.h>

namespace Nucleus {

    template<typename Type>
    class Fmt;
    
    template<class T>
    class Serializer;
    
    template<typename T>
    struct LowestType;

    template<typename T>
    struct LowestType {
        using Type = T;
    };

    template<template<typename> typename C, typename T>
    struct LowestType<C<T>> {
        using Type = typename LowestType<T>::Type;
    };

    template<typename T>
    using Lowest = typename LowestType<T>::Type;
    
    template <typename T, std::size_t = sizeof(T)>
    std::true_type implements(T*);

    std::false_type implements(...);

    template <typename T>
    using HasFmt = decltype(implements(std::declval<Fmt<T>*>()));

    template <typename T>
    using HasSerializer = decltype(implements(std::declval<Serializer<T>*>()));

    template <typename T>
    constexpr bool Serializable = HasSerializer<Lowest<T>>::value && HasSerializer<T>::value;
    
}

#define FMT_INLINE
#include <Nucleus/Inline/FmtImpl.inl>
#undef FMT_INLINE
