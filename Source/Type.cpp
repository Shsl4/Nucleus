#include <Nucleus/Type.h>
#include <Nucleus/String.h>

namespace Nucleus {

#ifdef __GNUG__

    String Type::demangle(const char *name) {

        int status = -1;

        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);

        if(status == 0){
            String className = demangled;
            free(demangled);
            return className;
        }

        return name;

    }

    class String Type::name(const std::type_info &type) {

        const char* className = type.name();

#ifdef __GNUG__
        return demangle(className);
#else
        return String(className).removeOccurrences("class ").removeOccurrences("struct ");
#endif

    }

#endif

}