#pragma once

#ifdef MUTABLE_INLINE

#include <Nucleus/Exceptions.h>
#include <Nucleus/String.h>

namespace Nucleus {

    template<class T>
    MutableArray<T>::MutableArray(MutableArray&& other) noexcept {
        *this = other;
    }

    template<class T>
    MutableArray<T>::MutableArray(const MutableArray &other) {
        *this = std::move(other);
    }


    template<class T>
    MutableArray<T>& MutableArray<T>::operator=(MutableArray &&other) noexcept {

        if(&other == this) return *this;

        Allocator<T>::release(buffer);

        this->buffer = other.buffer;
        this->bufferSize = other.bufferSize;
        this->bufferCapacity = other.bufferCapacity;

        other.buffer = nullptr;
        other.bufferSize = 0;
        other.bufferCapacity = 0;

        return *this;

    }

    template<class T>
    MutableArray<T>& MutableArray<T>::operator=(const MutableArray &other) {

        if(&other == this) return *this;

        const T* start = other.begin().get();

        if (start == nullptr) {
            this->bufferSize = 0;
            this->bufferCapacity = 0;
            return *this;
        }

        Allocator<T>::reallocate(buffer, bufferCapacity, other.bufferCapacity);
        Allocator<T>::copy(start, start + other.bufferCapacity, buffer);

        this->bufferSize = other.bufferSize;
        this->bufferCapacity = other.bufferCapacity;

        return *this;

    }

    template<class T>
    MutableArray<T>::MutableArray(size_t size) {

        this->buffer = Allocator<T>::allocate(size);
        this->bufferSize = 0;
        this->bufferCapacity = size;

    }

    template<class T>
    void MutableArray<T>::extend(const size_t size) {

        if(bufferSize + size > bufferCapacity){

            const size_t newSize = bufferCapacity > 0 ? bufferCapacity * 2 : size;
            Allocator<T>::reallocate(buffer, bufferCapacity, newSize);
            bufferCapacity = newSize;

        }

    }

    template<class T>
    bool MutableArray<T>::isEmpty() const {
        return bufferSize == 0;
    }

    template<class T>
    void MutableArray<T>::clear() {

        Allocator<T>::release(buffer);

        this->bufferSize = 0;
        this->bufferCapacity = 0;

    }

    template<class T>
    bool MutableArray<T>::contains(const T &element) const {

        for (size_t i = 0; i < bufferSize; ++i) {
            if (buffer[i] == element) return true;
        }

        return false;

    }

    template<class T>
    bool MutableArray<T>::remove(const T &element) {

        bool r = false;

        for(size_t i = 0; i < size(); ++i){

            if (buffer[i] == element){
                removeAt(i--);
                r = true;
            }

        }

        return r;

    }

    template<class T>
    bool MutableArray<T>::removeAt(size_t index) {

        if (index > size()) return false;

        Allocator<T>::move(buffer + index + 1, buffer + bufferSize, buffer + index);

        --bufferSize;

        return true;

    }

    template<class T>
    auto MutableArray<T>::insertAll(const Collection<T> &collection, size_t index) -> decltype(*this) & {

        if (index >= bufferSize) {
            return addAll(collection);
        }

        extend(collection.size());

        Allocator<T>::move(buffer + index, buffer + bufferSize, buffer + index + collection.size() + 1);

        for(T const& element : collection){

            buffer[index++] = element;
            ++bufferSize;

        }

        return *this;

    }

    template<class T>
    auto MutableArray<T>::insert(const T &element, size_t index) -> decltype(*this) & {

        if (index >= bufferSize) {
            return add(element);
        }

        extend();

        Allocator<T>::move(buffer + index, buffer + bufferSize, buffer + index + 1);

        buffer[index] = element;
        ++bufferSize;

        return *this;

    }

    template<class T>
    auto MutableArray<T>::addAll(const Collection<T> &collection) -> decltype(*this) & {

        extend(collection.size());

        for(T const& element : collection){
            buffer[bufferSize++] = element;
        }

        return *this;

    }

    template<class T>
    auto MutableArray<T>::add(const T &element) -> decltype(*this) & {

        extend();
        buffer[bufferSize++] = element;
        return *this;

    }

    template<class T>
    T &MutableArray<T>::get(size_t index) const {
        nassert(index < bufferSize);
        return buffer[index];
    }

    template<class T>
    size_t MutableArray<T>::size() const {
        return bufferSize;
    }

    template<class T>
    typename Collection<T>::Iterator MutableArray<T>::end() const {
        return typename Super::Iterator(buffer + size());
    }

    template<class T>
    typename Collection<T>::Iterator MutableArray<T>::begin() const {
        return typename Super::Iterator(buffer);
    }

    template<class T>
    size_t MutableArray<T>::capacity() const {
        return bufferCapacity;
    }

    template<class T>
    MutableArray<T>::MutableArray(std::initializer_list<T> list) {

        this->buffer = Allocator<T>::allocate(list.size());
        this->bufferSize = list.size();
        this->bufferCapacity = list.size();

        for (T const& element : list) {
            add(element);
        }

    }


    template<class T>
    auto MutableArray<T>::removeAll(const Collection<T> &collection) -> decltype(*this) & {

        for(auto const& e : collection){
            remove(e);
        }

        return *this;
    }


}

#endif
