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

        NODISCARD FORCEINLINE bool valid() const { return hasValue; }

        ~Optional() {
            if (hasValue) { storage.value.~T(); }
        }

        T *get() { return hasValue ? &storage.value : nullptr; }

        T const *get() const { return hasValue ? &storage.value : nullptr; }

        T &operator*() { nthrowif(!hasValue, "Trying to access an empty oprional!"); return storage.value; }

        T const &operator*() const { nthrowif(!hasValue, "Trying to access an empty oprional!"); return storage.value; }

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
