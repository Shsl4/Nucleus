#pragma once

#ifdef FMT_INLINE

#include <Nucleus/Nucleus.h>

namespace Nucleus {

    template<>
    class Fmt<String> {

    public:

        static String format(String const& string) {
            return string;
        }

        static String& format(String& string) {
            return string;
        }

    };

    template<>
    class Fmt<nullptr_t> {

    public:

        static String format(nullptr_t) {
            return "nullptr";
        }

    };

    template<typename Integral>
    class Fmt<Integral, EnableIf<std::is_integral_v<Integral>, Integral>> {

    public:

        static String format(Integral const& elem) {
            return String::fromInteger(elem);
        }


    };

    template<typename FloatingPoint>
    class Fmt<FloatingPoint, EnableIf<std::is_floating_point_v<FloatingPoint>, FloatingPoint>> {

    public:

        static String format(FloatingPoint elem) {
            return String::fromFloatingPoint(elem);
        }


    };

    template<typename Pointer>
    class Fmt<Pointer, EnableIf<std::is_pointer_v<Pointer>, Pointer>> {

    public:

        static String format(Pointer elem) {
            return String::fromPointer(elem);
        }


    };

    template<>
    class Fmt<bool> {

    public:

        static String format(const bool elem) {
            return elem ? "true" : "false";
        }


    };

    template<>
    class Fmt<const char*> {

    public:

        static String format(const char* elem) {
            return elem;
        }


    };

    template<size_t n>
    class Fmt<char[n]> {

    public:

        static String format(const char elem[n]) {
            return elem;
        }

    };

    template<>
    class Fmt<const wchar_t*> {

    public:

        static String format(const wchar_t* elem) {
            return elem;
        }


    };

    template<size_t n>
    class Fmt<wchar_t[n]> {

    public:

        static String format(const wchar_t elem[n]) {
            return elem;
        }

    };

    template<typename T>
    class Fmt<Shared<T>> {

    public:

        static String format(Shared<T> const& elem) {
            return elem.isValid() ? Fmt<T>::format(*elem) : "empty";
        }

    };

    template<typename T>
    class Fmt<Weak<T>> {

    public:

        static String format(Weak<T> const& elem) {
            return elem.isValid() ? Fmt<T>::format(*elem) : "empty";
        }

    };

    template<typename T>
    class Fmt<Unique<T>> {

    public:

        static String format(Unique<T> const& elem) {
            return elem.isValid() ? Fmt<T>::format(*elem) : "empty";
        }

    };

}

#endif