#pragma once

#include <Nucleus/Nucleus.h>
#include <Nucleus/Function.h>
#include <Nucleus/Map.h>

namespace Nucleus {

class ReflectionFactory {

public:

    static Any createObject(String const& className) {

        try{
            return constructors[className].invoke();
        }
        catch (Exceptions::BadKey const& key){
            return Any::empty();
        }

    }

    static void registerConstructor(Nucleus::String const& className, Function<Any()> const& constructor){
        constructors.add(className, constructor);
    }

private:

    static inline Map<String, Function<Any()>> constructors = {};

};

}

#define REFLECTABLE(c)                                                                              \
                                                                                                    \
    namespace ReflectionInitializers {                                                              \
                                                                                                    \
        inline struct c##Initializer {                                                              \
            c##Initializer() { Nucleus::ReflectionFactory::registerConstructor(#c,                  \
                []() { return Nucleus::Any(c()); }); }                                              \
        } init##c;                                                                                  \
                                                                                                    \
    }

#define TEMPLATE_REFLECTABLE(c, t)                                                                  \
                                                                                                    \
    namespace ReflectionInitializers {                                                              \
                                                                                                    \
        inline struct c##t##Initializer {                                                           \
            c##t##Initializer() { Nucleus::ReflectionFactory::registerConstructor(#c"<"#t">",       \
                []() { return Nucleus::Any(c<t>()); }); }                                           \
        } init##c##t;                                                                               \
                                                                                                    \
    }                                                                                               \

