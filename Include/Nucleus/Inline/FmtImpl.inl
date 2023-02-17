#pragma once

#ifdef FMT_INLINE

#include <Nucleus/Unique.h>
#include <Nucleus/Shared.h>

namespace Nucleus {

    template<>
    class Fmt<String> {

    public:

        static String format(String const& string, String const& params) {
            return string;
        }

        static String& format(String& string, String const& params) {
            return string;
        }

    };

    template<>
    class Fmt<nullptr_t> {

    public:

        static String format(nullptr_t, String const& params) {
            return "nullptr";
        }

    };

    template<typename Integral> requires std::is_integral_v<Integral>
    class Fmt<Integral> {

    public:

        static String format(Integral const& elem, String const& params) {

            size_t base = 10;
            if (params == "x") base = 16;
            if (params == "b") base = 2;

            return String::fromInteger(elem, base);
        }


    };

    template<typename FloatingPoint> requires std::is_floating_point_v<FloatingPoint>
    class Fmt<FloatingPoint> {

    public:

        static String format(FloatingPoint elem, String const& params) {
            return String::fromFloatingPoint(elem);
        }


    };

    template<typename Pointer> requires std::is_pointer_v<Pointer>
    class Fmt<Pointer> {

    public:

        static String format(Pointer elem, String const& params) {
            return String::fromPointer(elem);
        }


    };

    template<>
    class Fmt<bool> {

    public:

        static String format(const bool elem, String const& params) {
            return elem ? "true" : "false";
        }


    };

    template<>
    class Fmt<const char*> {

    public:

        static String format(const char* elem, String const& params) {
            return elem;
        }


    };

    template<size_t n>
    class Fmt<char[n]> {

    public:

        static String format(const char elem[n], String const& params) {
            return elem;
        }

    };

    template<typename T>
    class Fmt<Shared<T>> {

    public:

        static String format(Shared<T> const& elem, String const& params) {
            return elem.isValid() ? Fmt<T>::format(*elem, params) : "empty";
        }

    };

    template<typename T>
    class Fmt<Weak<T>> {

    public:

        static String format(Weak<T> const& elem, String const& params) {
            return elem.isValid() ? Fmt<T>::format(*elem, params) : "empty";
        }

    };

    template<typename T>
    class Fmt<Unique<T>> {

    public:

        static String format(Unique<T> const& elem, String const& params) {
            return elem.isValid() ? Fmt<T>::format(*elem, params) : "empty";
        }

    };

    template<>
    class Fmt<std::type_info> {

    public:

        static String format(std::type_info const& type, String const& params) {
            return Type::name(type);
        }

    };

    template<template<typename, size_t> typename ArrayType, typename T, size_t sz>
    class Fmt<ArrayType<T, sz>> {

    public:

        static String format(ArrayType<T, sz> const& array, String const& params) {

            if (array.isEmpty()) return "[]";

            String result = "[";

            using Iterator = typename ArrayType<T, sz>::Iterator;
            using Type = typename Iterator::value_type;

            for(size_t i = 0; i < array.size() - 1; ++i){
                result += Fmt<Type>::format(array[i], params) + ", ";
            }

            result += Fmt<Type>::format(array[array.size() - 1], params) + "]";

            return result;

        }

    };

    template<template<typename> typename CollectionType, typename T> requires (std::is_base_of_v<Collection<T>, CollectionType<T>>)
    class Fmt<CollectionType<T>> {

    public:

        static String format(CollectionType<T> const& array, String const& params) {

            if (array.isEmpty()) return "[]";

            String result = "[";
            
            using Iterator = typename CollectionType<T>::Iterator;
            using Type = typename Iterator::value_type;

            for(size_t i = 0; i < array.size() - 1; ++i){
                result += Fmt<Type>::format(array[i], params) + ", ";
            }
            
            result += Fmt<Type>::format(array[array.size() - 1], params) + "]";
            
            return result;
            
        }

    };

}

#endif
