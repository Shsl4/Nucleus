#pragma once

#ifdef TYPE_INLINE

#include <Nucleus/String.h>

namespace Nucleus {

    template<typename T>
    String Type::name(const T &object) {

        const char* className = typeid(object).name();

#ifdef __GNUG__
        return demangle(className);
#else
        return String(className).removeOccurrences("class ").removeOccurrences("struct ");
#endif

    }

    template<typename T>
    String Type::name() {

        const char* className = typeid(T).name();

#ifdef __GNUG__
        return demangle(className);
#else
        return String(className).removeOccurrences("class ").removeOccurrences("struct ");
#endif

    }


    template<typename T>
    std::type_info const& Type::of() {
        return typeid(T);
    }

    template<typename T>
    std::type_info const& Type::of(const T &object) {
        return typeid(object);
    }

}

#endif
