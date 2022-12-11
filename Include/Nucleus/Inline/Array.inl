#pragma once

#ifdef ARRAY_INLINE

#include <Nucleus/Exceptions.h>
#include <Nucleus/String.h>

namespace Nucleus {

    template <class T>
    Collection<T>::Iterator::Iterator(pointer ptr): ptr(ptr) {}

    template <class T>
    typename Collection<T>::Iterator::reference Collection<T>::Iterator::operator*() const {
        return *ptr;
    }

    template <class T>
    typename Collection<T>::Iterator::pointer Collection<T>::Iterator::operator->() {
        return ptr;
    }

    template <class T>
    typename Collection<T>::Iterator::pointer Collection<T>::Iterator::get() {
        return ptr;
    }

    template <class T>
    typename Collection<T>::Iterator::reference Collection<T>::Iterator::operator++() { ++ptr; return *ptr; }

    template<class T>
    T &Collection<T>::operator[](size_t index) const {
        return get(index);
    }

}

#endif
