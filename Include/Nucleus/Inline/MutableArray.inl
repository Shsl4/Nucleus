#pragma once

#include <Nucleus/Exceptions.h>

#ifdef MUTABLE_INLINE

#include <Nucleus/ImmutableArray.h>

namespace Nucleus {

    template <typename T>
    MutableArray<T>::~MutableArray() {

        this->count = 0;
        this->capacity = 0;
        Allocator<T>::release(buffer);

    }

    template <typename T>
    MutableArray<T>::MutableArray(size_t cap): capacity(cap) {
        this->buffer = Allocator<T>::allocate(capacity);
    }

    template <typename T>
    MutableArray<T>::MutableArray(std::initializer_list<T> list) {

        this->capacity = list.size();
        this->count = list.size();
        this->buffer = Allocator<T>::allocate(capacity);
        Allocator<T>::copy(list.begin(), list.end(), buffer);

    }

    template <typename T>
    MutableArray<T>::MutableArray(T* buf, size_t size) : count(buf ? size : 0), capacity(count) {

        if(!buf) { return; }

        this->buffer = Allocator<T>::allocate(size);

        Allocator<T>::copy(buf, buf + size, this->buffer);
            
    }

    template <typename T>
    MutableArray<T>::MutableArray(MutableArray const& other) {
        *this = other;
    }

    template <typename T>
    MutableArray<T>::MutableArray(MutableArray&& other) noexcept {
        *this = std::move(other);
    }

    template <typename T>
    T& MutableArray<T>::operator[](size_t index) {
        if(index >= count) throw Exceptions::OutOfRange("MutableArray index out of range");
        return buffer[index];
    }

    template <typename T>
    T& MutableArray<T>::operator[](size_t index) const {
        if(index >= count) throw Exceptions::OutOfRange("MutableArray index out of range");
        return buffer[index];
    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::operator+=(T const& element) {
        return append(element);
    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::operator+=(MutableArray const& array) {
        return append(array);
    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::operator+=(T&& element) {
        return append(std::move(element));
    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::operator=(MutableArray const& other) {

        if(&other == this) return *this;
        
        const T* start = other.begin().get();

        if (start == nullptr) {
            this->count = 0;
            this->capacity = 0;
            return *this;
        }

        Allocator<T>::reallocate(buffer, capacity, other.capacity);
        Allocator<T>::copy(start, start + other.capacity, buffer);

        this->count = other.count;
        this->capacity = other.capacity;
        return *this;
            
    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::operator=(MutableArray&& other) noexcept {

        if(&other == this) return *this;

        Allocator<T>::release(buffer);

        this->buffer = other.buffer;
        this->count = other.count;
        this->capacity = other.capacity;

        other.buffer = nullptr;
        other.count = 0;
        other.capacity = 0;
            
        return *this;
            
    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::append(T const& element) {

        checkSize();
        buffer[count] = element;
        ++count;
        return *this;

    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::append(T&& element) {

        checkSize();
        buffer[count] = std::move(element);
        ++count;
        return *this;

    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::append(MutableArray const& array) {

        checkSize(array.count);

        Allocator<T>::copy(array.buffer, array.buffer + array.count, buffer + count);

        count += array.count;

        return *this;

    }

    template <typename T>
    bool MutableArray<T>::removeAt(size_t index) {

        if (index >= count) return false;

        Allocator<T>::move(buffer + index + 1, buffer + count, buffer + index);

        --count;

        return true;

    }

    template <typename T>
    bool MutableArray<T>::removeAllOf(T const& value) {

        bool removed = false;

        for (size_t index = count; index > 0; --index) {

            if(buffer[index - 1] == value){
                Allocator<T>::move(buffer + index, buffer + count, buffer + index - 1);
                removed = true;
                --count;
            }

        }

        return removed;

    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::insert(T const& element, size_t index) {

        if (index >= count) {
            return append(element);
        }

        checkSize();

        Allocator<T>::move(buffer + index, buffer + count, buffer + index + 1);

        buffer[index] = element;

        ++count;

        return *this;

    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::insert(T&& element, size_t index) {

        if (index >= count) {
            return append(element);
        }

        checkSize();

        Allocator<T>::move(buffer + index, buffer + count, buffer + index + 1);

        buffer[index] = std::move(element);

        ++count;

        return *this;

    }

    template <typename T>
    MutableArray<T>& MutableArray<T>::insert(MutableArray const& array, size_t index) {

        if (index >= count) {
            return append(array);
        }

        checkSize(array.count);

        Allocator<T>::move(buffer + index, buffer + count, buffer + index + array.count);

        Allocator<T>::copy(array.buffer, array.buffer + array.count, buffer + index);

        return *this;

    }

    template <typename T>
    typename MutableArray<T>::Iterator MutableArray<T>::begin() const { return Iterator(buffer); }

    template <typename T>
    typename MutableArray<T>::Iterator MutableArray<T>::end() const { return Iterator(buffer + size()); }

    template <typename T>
    void MutableArray<T>::checkSize(const size_t size) {

        if(count + size > capacity){
                
            const size_t newSize = capacity > 0 ? capacity * 2 : size;
            Allocator<T>::reallocate(buffer, capacity, newSize);
            capacity = newSize;
                
        }

    }

    template <typename T>
    ImmutableArray<T> MutableArray<T>::toImmutable() const {

        return ImmutableArray<T>(buffer, count);
        
    }

    template <typename T>
    void MutableArray<T>::reset() {
        this->count = 0;
    }

    template <typename T>
    MutableArray<T>::Iterator::Iterator(pointer ptr): ptr(ptr) {

    }

    template <typename T>
    typename MutableArray<T>::Iterator::reference MutableArray<T>::Iterator::operator*() const {
        return *ptr;
    }

    template <typename T>
    typename MutableArray<T>::Iterator::pointer MutableArray<T>::Iterator::operator->() {
        return ptr;
    }

    template <typename T>
    typename MutableArray<T>::Iterator::pointer MutableArray<T>::Iterator::get() {
        return ptr;
    }

    template <typename T>
    typename MutableArray<T>::Iterator::reference MutableArray<T>::Iterator::operator++() { ++ptr; return *ptr; }
    
}

#endif
