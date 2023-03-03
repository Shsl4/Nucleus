#pragma once

#include <Nucleus/Exceptions.h>

namespace Nucleus {

    template <typename T>
    class Optional {

    public:

        Optional() = default;

        explicit Optional(T const &value) : storage(), hasValue(true) {

            new(&storage.value) T(value);

        }

        explicit Optional(T &&value) : storage(), hasValue(true) {

            new(&storage.value) T(std::move(value));

        }

        ~Optional() {
            if (hasValue) { storage.value.~T(); }
        }

        T *get() { return hasValue ? &storage.value : nullptr; }

        T const *get() const { return hasValue ? &storage.value : nullptr; }

        T &operator*() { return *get(); }

        T const &operator*() const { return *get(); }

        T* operator->() { return get(); }

        T const* operator->() const { return get(); }

        explicit operator bool() const { return hasValue; }

    private:

        struct Empty {};

        union Storage {

            Empty empty;
            T value;

            Storage() : empty() {}
            ~Storage() {}

        };

        Storage storage;
        bool hasValue = false;

    };

}
