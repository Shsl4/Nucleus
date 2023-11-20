#pragma once

#include <Nucleus/Nucleus.h>
#include <Nucleus/Function.h>
#include <Nucleus/Map.h>

namespace Nucleus {
    
    template<typename T>
    struct RawType;

    template<typename T>
    struct RawType {
        using Type = T;
    };

    template<typename T>
    struct RawType<T&> {
        using Type = typename RawType<std::remove_reference_t<T>>::Type;
    };

    template<typename T>
    struct RawType<T&&> {
        using Type = typename RawType<std::remove_reference_t<T>>::Type;
    };

    template<typename T>
    struct RawType<T*> {
        using Type = typename RawType<std::remove_pointer_t<T>>::Type;
    };

    template<typename T>
    struct RawType<T* const> {
        using Type = typename RawType<std::remove_pointer_t<T>>::Type;
    };

    template<typename T>
    using Raw = typename RawType<T>::Type;

    class ReflectionFactory {

    public:

        static Any createObject(String const& className) {

            try{
                return constructors[className]();
            }
            catch (Exceptions::BadKey const&){
                return Any::empty();
            }

        }

        static void* allocateObject(String const& className) {

            try{
                return pointerConstructors[className]();
            }
            catch (Exceptions::BadKey const&){
                return nullptr;
            }

        }

        static bool isReflected(String const& className) {
            return constructors.containsKey(className) && constructors.containsKey(className);
        }
    
        template<class T>
        static void registerClass(){
            constructors.add(Type::name<T>(), []{ return Any(T()); });
            pointerConstructors.add(Type::name<T>(), []{ return Allocator<T>::construct(); });
        }

    private:

        static inline Map<String, Function<Any()>> constructors = {};
        static inline Map<String, Function<void*()>> pointerConstructors = {};

    };

    class Class {

    public:

        Class() = delete;

        static Class fromName(String const& name) {

            nthrowif(!ReflectionFactory::isReflected(name), "The target class is not reflected!");
            return Class(name);
        
        }

        template<class T>
        static Class of(T const& object) {

            nthrowif(!ReflectionFactory::isReflected(nameRemovePointer(object)), "The target class is not reflected!");
            return Class(nameRemovePointer(object));
        
        }
        
        NODISCARD Any createInstance() const {
            return ReflectionFactory::createObject(className);
        }

        template<class T>
        NODISCARD T* unmanaged() const {
            nthrowif(Type::name<T>() != className, "Bad class type");
            return static_cast<T*>(ReflectionFactory::allocateObject(className));
        }

        NODISCARD void* unmanaged() const {
            return ReflectionFactory::allocateObject(className);
        }

        template<class T>
        NODISCARD Unique<T> unique() const {
            nthrowif(Type::name<T>() != className, "Bad class type");
            return Unique<T>(unmanaged<T>());
        }

        NODISCARD FORCEINLINE String name() const { return this->className; }

        bool operator==(Class const& other) const { return className == other.className; }

    private:

        template<typename T> requires (!std::is_pointer_v<T>)
        static String nameRemovePointer(T const& elem) {
            return Type::name(elem);
        }

        template<typename T> requires std::is_pointer_v<T>
        static String nameRemovePointer(T const& elem) {
            return elem ? nameRemovePointer(*elem) : Type::name<Raw<T>>();
        }

        explicit Class(String name) : className(std::move(name)) {
        
        }

        const String className = "";
    
    };

}

#define REFL_BODY(c)                                                                                            \
    struct Refl {                                                                                               \
    private:                                                                                                    \
        inline static struct Init {                                                                             \
            Init() { Nucleus::ReflectionFactory::registerClass<c>(); }                                          \
        } __refl_init;                                                                                          \
    };                                                                                                          \
