#pragma once

#ifdef COLLECTION_INLINE

#include <Nucleus/Random.h>
#include <Nucleus/Allocator.h>

namespace Nucleus {

    template <class T>
    Collection<T>::Iterator::Iterator(IteratorBase* it): it(it) {}

    template <class T>
    typename Collection<T>::Iterator::reference Collection<T>::Iterator::operator*() const {
        return it->operator*();
    }

    template <class T>
    typename Collection<T>::Iterator::pointer Collection<T>::Iterator::operator->() {
        return it->operator->();
    }

    template <class T>
    typename Collection<T>::Iterator::pointer Collection<T>::Iterator::get() {
        return it->get();
    }

    template <class T>
    typename Collection<T>::Iterator::reference Collection<T>::Iterator::operator++() { return it->operator++(); }

    template<class T>
    T &Collection<T>::operator[](size_t index) const {
        return get(index);
    }

    template<class T>
    void Collection<T>::randomize() {

        for (size_t i = 0; i < size(); ++i) {
            Allocator<T>::swap(get(i), get(Nucleus::Random::randomInteger<size_t>(0, size() - 1)));
        }

    }

}

#endif
