#pragma once

#include <Nucleus/Unique.h>

#ifdef UNIQUE_INLINE

namespace Nucleus{


    template<typename T>
    T &Unique<T>::operator*() { assertValid(); return *object; }

    template<typename T>
    T const &Unique<T>::operator*() const { assertValid(); return *object; }

    template<typename T>
    T *Unique<T>::operator->() { assertValid(); return object; }

    template<typename T>
    T *Unique<T>::operator->() const { assertValid(); return object; }

    template<typename T>
    String Unique<T>::getClass() const {
        return isValid() ? Type::name(*object) : "nullptr";
    }

    template<typename T>
    Unique<T>& Unique<T>::operator=(Unique &&other) noexcept {

        if(&other == this) return *this;

        this->object = other.object;
        other.object = nullptr;
        return *this;

    }

    template<typename T>
    Unique<T>::Unique(Unique &&other) noexcept { *this = std::move(other); }

    template<typename T>
    template<typename... Args>
    Unique<T> Unique<T>::make(Args &&... args) {

        Unique<T> unique;
        unique.object = Allocator<T>::construct(std::forward<Args>(args)...);
        return unique;

    }

    template<typename T>
    Unique<T>::~Unique() {
        drop();
    }

    template<typename T>
    Unique<T>::Unique(nullptr_t) {
    }

    template<typename T>
    void Unique<T>::drop() {
        Allocator<T>::destroy(object);
    }

    template<typename T>
    bool Unique<T>::operator==(const Unique &other) const {
        return object != nullptr && object == other.object;
    }

    template<typename T>
    Unique<T>& Unique<T>::operator=(nullptr_t) {

        drop();
        return *this;

    }

    template<typename T>
    Unique<T>::operator bool() const {
        return isValid();
    }

    template<typename T>
    bool Unique<T>::operator==(const T &other) const {
        if(!isValid()) return false;
        return *object == other;
    }

}

#endif
