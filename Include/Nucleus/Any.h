#pragma once

#include <Nucleus/Shared.h>

namespace Nucleus {

    class Any {

    public:

        Any() = default;

        template <typename T> requires (!std::is_base_of_v<Any, T>)
        explicit Any(T const& value) : pointer(Shared<Derived<T>>::make(value)) {

        }

        template <typename T> requires (!std::is_base_of_v<Any, T>)
        explicit Any(T&& value) : pointer(Shared<Derived<T>>::make(std::forward<T>(value))) {

        }

        static Any empty() { return {}; }

        template <typename T>
        bool isOfType(){
            if (!pointer) return false;
            return pointer->type() == Type::of<T>();
        }

        template <typename T>
        T* get() {

            if (isOfType<T>()) {
                return static_cast<T*>(pointer.pointer()->getValue());
            }

            return nullptr;

        }

        template <typename T>
        T const* get() const {

            if (isOfType<T>()) {
                return static_cast<T*>(pointer.pointer()->getValue());
            }

            return nullptr;

        }

        NODISCARD std::type_info const& type() const {
            return pointer ? pointer->type() : Type::of<void>();
        }

    private:

        struct Base {
            NODISCARD virtual std::type_info const& type() const = 0;
            NODISCARD virtual void* getValue() const = 0;
            virtual ~Base() = default;
        };

        template <typename T>
        struct Derived : Base {

            explicit Derived(T&& value) : value(std::forward<T>(value)) {}
            explicit Derived(T const& value) : value(value) {}
            NODISCARD const std::type_info &type() const override { return Nucleus::Type::of<T>(); }
            NODISCARD void* getValue() const override { return const_cast<T*>(&value); }
            T value;

        };

        Shared<Base> pointer;

    };

}