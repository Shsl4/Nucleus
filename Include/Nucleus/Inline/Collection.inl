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

    template <class T>
    bool Collection<T>::removeFirst(T const& element) {

        for(size_t i = 0; i < size(); ++i){
            if (get(i) == element){ return removeAt(i); }
        }

        return false;
            
    }

    template <class T>
    bool Collection<T>::removeLast(T const& element) {

        for(size_t i = size(); i > 0; --i){
            if (get(i - 1) == element){ return removeAt(i - 1); }
        }

        return false;
            
    }

    template <class T>
    auto Collection<T>::remove(Collection const& collection) -> decltype(*this)& {

        for(auto const& element : collection){ this->removeFirst(element); }
            
        return *this;
            
    }

    template <class T>
    auto Collection<T>::removeAll(T const& element) -> decltype(*this)& {

        for(size_t i = 0; i < size(); ++i){
            if (get(i) == element){ removeAt(i--); }
        }

        return *this;
            
    }

    template <class T>
    bool Collection<T>::contains(T const& element) const {
            
        for (size_t i = 0; i < size(); ++i) {
            if (get(i) == element) {
                return true;
            }
        }
            
        return false;
    }

    template<class T>
    T &Collection<T>::operator[](size_t index) const {
        return get(index);
    }

    template<class T>
    void Collection<T>::randomize() {

        for (size_t i = 0; i < size(); ++i) {
            Allocator<T>::swap(get(i), get(Random::randomInteger<size_t>(0, size() - 1)));
        }

    }

}

#endif
