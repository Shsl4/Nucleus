#pragma once

#include <utility>
#include <Nucleus/Nucleus.h>
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
            nassertf(function.isOfType<FunctionType>(), "Bad invocation type");
            return (*function.get<FunctionType>())(std::forward<Args>(args)...);

        }

        NODISCARD String typeName() const {
            return Type::name(function.type());
        }

    private:

        Nucleus::Any function;

    };

    template<typename ReturnType, typename... Args>
    class Function<ReturnType(Args...)> {

        using FunctionType = ReturnType(*)(Args...);

        FunctionType func = nullptr;

    public:

        template<size_t n> requires (sizeof...(Args) > 0 && n < sizeof...(Args))
        using ArgType = std::tuple_element_t<n, std::tuple<Args...>>;

        Function() = default;

        Function(FunctionType func) : func(func) {

        }

        template<class Lambda>
        Function(Lambda&& lambda) : func(lambda) {
            static_assert(std::is_invocable_r_v<ReturnType, Lambda, Args...>);
        }

        ReturnType operator()(Args&&... args){
            return func(std::forward<Args>(args)...);
        }

        ReturnType invoke(Args&&... args){
            return func(std::forward<Args>(args)...);
        }

        NODISCARD ErasedFunction erased() const {
            return ErasedFunction(func);
        }

    };


    template<class Class, typename ReturnType, typename... Args>
    class Function<ReturnType((Class::*)(Args...))>{

        using FunctionType = ReturnType(Class::*)(Args...);

        FunctionType func = nullptr;
        Class* target = nullptr;

    public:

        template<size_t n> requires (sizeof...(Args) > 0 && n < sizeof...(Args))
        using ArgType = std::tuple_element_t<n, std::tuple<Args...>>;

        Function() = default;

        Function(FunctionType func) : func(func) {

        }

        void bindTarget(Class* object){
            this->target = object;
        }

        ReturnType operator()(Args&&... args){
            nassertf(target, "Trying to invoke a function without a target");
            return (target->*func)(std::forward<Args>(args)...);
        }

        ReturnType invoke(Args&&... args){
            nassertf(target, "Trying to invoke a function without a target");
            return (target->*func)(std::forward<Args>(args)...);
        }

        ReturnType invokeWith(Class* object, Args&&... args){
            nassertf(object, "Trying to invoke a function with an invalid object");
            return (object->*func)(std::forward<Args>(args)...);
        }

        ReturnType invokeWith(Class& object, Args&&... args){
            return (object.*func)(std::forward<Args>(args)...);
        }

    };


}