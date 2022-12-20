#pragma once

#ifdef FMT_INLINE

#include <Nucleus/Unique.h>
#include <Nucleus/Shared.h>

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

    template<typename Integral> requires std::is_integral_v<Integral>
    class Fmt<Integral> {

    public:

        static String format(Integral const& elem) {
            return String::fromInteger(elem);
        }


    };

    template<typename FloatingPoint> requires std::is_floating_point_v<FloatingPoint>
    class Fmt<FloatingPoint> {

    public:

        static String format(FloatingPoint elem) {
            return String::fromFloatingPoint(elem);
        }


    };

    template<typename Pointer> requires std::is_pointer_v<Pointer>
    class Fmt<Pointer> {

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

    template<>
    class Fmt<std::type_info> {

    public:

        static String format(std::type_info const& type) {
            return Type::name(type);
        }

    };

}

#endif
