#pragma once

#ifdef TYPE_INLINE

#include <Nucleus/String.h>

namespace Nucleus{

    template<typename T>
    String Type::name(const T &object) {

        const char* className = typeid(object).name();

#ifdef __GNUG__
        return demangle(className);
#else
        auto str = String(className);
        str.removeOccurrences("class ");
        str.removeOccurrences("struct ");
        return str;
#endif

    }

    template<typename T>
    String Type::name() {

        const char* className = typeid(T).name();

#ifdef __GNUG__
        return demangle(className);
#else
        auto str = String(className);
        str.removeOccurrences("class ");
        str.removeOccurrences("struct ");
        return str;
#endif

    }

#ifdef __GNUG__
    
    inline String Type::demangle(const char *name) {

        int status = -1;

        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);

        if(status == 0){
            String className = demangled;
            free(demangled);
            return className;
        }

        return name;

    }

    inline class String Type::name(const std::type_info &type) {

        const char* className = type.name();

#ifdef __GNUG__
        return demangle(className);
#else
        auto str = String(className);
        str.removeOccurrences("class ");
        str.removeOccurrences("struct ");
        return str;
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

#endif

}

#endif
