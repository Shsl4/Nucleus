#pragma once

#include <utility>
#include <Nucleus/Any.h>

namespace Nucleus {

    template<typename T>
    class Function;

    class ErasedFunction {

    public:

        ErasedFunction() = default;

        template<typename T, typename... Args>
        ErasedFunction(T(*func)(Args...)) : function(Nucleus::Any(std::move(func))) {}

        template<typename T, typename... Args>
        T invoke(Args&&... args) {

            using FunctionType = T(*)(Args...);
            
            if(!function.isOfType<FunctionType>()){
                nthrow("Bad invocation type");
            }
            
            return (*function.get<FunctionType>())(std::forward<Args>(args)...);

        }

        NODISCARD String typeName() const {
            return Type::name(function.type());
        }

    private:

        Any function;

    };

    template<typename ReturnType, typename... Args>
    class Function<ReturnType(Args...)> {

        using FunctionType = ReturnType(*)(Args...);

        FunctionType function = nullptr;

        friend class Fmt<Function>;

    public:

        template<size_t n> requires (sizeof...(Args) > 0 && n < sizeof...(Args))
        using ArgType = std::tuple_element_t<n, std::tuple<Args...>>;

        Function() = default;

        Function(FunctionType func) : function(func) {

        }

        template<class Lambda> requires !std::is_base_of_v<Function, FunctionType>
        Function(Lambda&& lambda) : function(lambda) {
            static_assert(std::is_invocable_r_v<ReturnType, Lambda, Args...>);
        }

        ReturnType operator()(Args&&... args){
            return function(std::forward<Args>(args)...);
        }

        ReturnType invoke(Args&&... args){
            return function(std::forward<Args>(args)...);
        }

        NODISCARD ErasedFunction erased() const {
            return ErasedFunction(function);
        }

    };

    template<typename T>
    class Fmt<Function<T>> {

    public:

        static String format(Function<T> const& f, String const& params) {
            return Type::name(f) + " at " +
                Fmt<const void*>::format(*(const void**)(reinterpret_cast<const void*>(&f.func)), params);
        }

    };

    template<class Class, typename ReturnType, typename... Args>
    class Function<ReturnType(Class::*)(Args...)>{

        using FunctionType = ReturnType(Class::*)(Args...);

        FunctionType function = nullptr;
        Class* target = nullptr;

        friend class Fmt<Function>;

    public:

        template<size_t n> requires (sizeof...(Args) > 0 && n < sizeof...(Args))
        using ArgType = std::tuple_element_t<n, std::tuple<Args...>>;

        Function() = default;

        Function(FunctionType func) : function(func) {

        }

        void bindTarget(Class* object){
            this->target = object;
        }

        ReturnType operator()(Args&&... args){
            nassertf(target, "Trying to invoke a function without a target");
            return (target->*function)(std::forward<Args>(args)...);
        }

        ReturnType invoke(Args&&... args){
            nassertf(target, "Trying to invoke a function without a target");
            return (target->*function)(std::forward<Args>(args)...);
        }

        ReturnType invokeWith(Class* object, Args&&... args){
            nassertf(object, "Trying to invoke a function with an invalid object");
            return (object->*function)(std::forward<Args>(args)...);
        }

        ReturnType invokeWith(Class& object, Args&&... args){
            return (object.*function)(std::forward<Args>(args)...);
        }

    };


}
