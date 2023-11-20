#pragma once

#include <utility>
#include <Nucleus/Any.h>

namespace Nucleus {

    template<typename T>
    class Function;

    template<typename ReturnType, typename... Args>
    class Function<ReturnType(Args...)> {

        friend class Fmt<Function>;

    public:

        template<size_t n> requires (sizeof...(Args) > 0 && n < sizeof...(Args))
        using ArgType = std::tuple_element_t<n, std::tuple<Args...>>;

        using FunctionType = ReturnType(*)(Args...);

        Function() = default;

        explicit operator bool() const { return callable; }

        bool operator==(Function const& other) const{
            return this->callable == other.callable;
        }

        Function(nullptr_t) {

        }

        Function(FunctionType func) : callable(Shared<Derived<FunctionType>>::make(func)) {

        }

        template<class LambdaType> requires std::is_invocable_r_v<ReturnType, LambdaType, Args...>
        Function(LambdaType lambda) : callable(Shared<Derived<LambdaType>>::make(lambda)) {

        }

        FORCEINLINE ReturnType operator()(Args... args) const {
            return callable.pointer()->operator()(std::forward<Args>(args)...);
        }

    private:

        struct Base {

            virtual ~Base() = default;

            FORCEINLINE virtual ReturnType operator()(Args&&... args) { nthrow("Illegal state"); }

        };

        template<typename T>
        struct Derived : Base {

            explicit Derived(T&& value) : value(std::forward<T>(value)) {}

            explicit Derived(T const& value) : value(value) {}

            FORCEINLINE ReturnType operator()(Args&&... args) override { return value(std::forward<Args>(args)...); }

            T value;

        };

        Shared<Base> callable = nullptr;

    };

    template<class Class, typename ReturnType, typename... Args>
    class Function<ReturnType(Class::*)(Args...)>{

        friend class Fmt<Function>;

    public:

        using FunctionType = ReturnType(Class::*)(Args...);
        using ConstFunctionType = ReturnType(Class::*)(Args...) const;

        template<size_t n> requires (sizeof...(Args) > 0 && n < sizeof...(Args))
        using ArgType = std::tuple_element_t<n, std::tuple<Args...>>;

        Function() = default;

        explicit operator bool() const { return function != nullptr; }

        Function(FunctionType func) : function(func) {

        }


        Function(ConstFunctionType func) : function((FunctionType)(func)){

        }

        void bindTarget(Class* object) { this->target = object; }

        ReturnType operator()(Args... args) const {
            assert(target);
            return (target->*function)(std::forward<Args>(args)...);
        }

        ReturnType operator()(Class& object, Args... args) const {
            return (object.*function)(std::forward<Args>(args)...);
        }

    private:

        FunctionType function = nullptr;
        Class* target = nullptr;

    };

}
