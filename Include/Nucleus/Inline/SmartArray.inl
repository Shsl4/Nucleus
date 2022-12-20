#pragma once

#ifdef SMARTARRAY_INLINE

namespace Nucleus {


    template<class T>
    void SmartArray<T>::extend(size_t size) {

        if(arraySize + size > arrayCapacity){

            const size_t newSize = arrayCapacity > 0 ? arrayCapacity * 2 : 5;
            Allocator<T*>::reallocate(buffer, arrayCapacity, newSize);
            arrayCapacity = newSize;

        }

    }

    template<class T>
    void SmartArray<T>::decref() {

        if (!counter) return;

        counter->unref();

        if (counter->exhausted()){

            Allocator<RefCounter>::destroy(counter);
            clear();

        }

    }

    template<class T>
    SmartArray<T>::SmartArray() : arraySize(0), arrayCapacity(10), buffer(Allocator<T*>::allocate(arrayCapacity)),
                                  counter(Allocator<RefCounter>::construct()) {

        this->counter->ref();

    }

    template<class T>
    SmartArray<T>::SmartArray(SmartArray &&other) noexcept {

        this->buffer = other.buffer;
        this->counter = other.counter;
        this->arraySize = other.arraySize;
        this->arrayCapacity = other.arrayCapacity;

        other.buffer = nullptr;
        other.counter = nullptr;
        other.arraySize = 0;
        other.arrayCapacity = 0;

    }

    template<class T>
    SmartArray<T>::~SmartArray() {
        decref();
    }

    template<class T>
    SmartArray<T>::SmartArray(const SmartArray &other) {
        *this = other;
    }

    template<class T>
    SmartArray<T>& SmartArray<T>::operator=(const SmartArray &other) {

        if(&other == this) return *this;

        decref();

        Allocator<T*>::release(buffer);

        if (other.capacity() == 0) return *this;

        this->buffer = Allocator<T*>::allocate(other.capacity());
        Allocator<T*>::copy(other.buffer, other.buffer + other.arraySize, this->buffer);

        this->arraySize = other.arraySize;
        this->arrayCapacity = other.arrayCapacity;
        this->counter = other.counter;

        this->counter->ref();

        return *this;

    }

    template<class T>
    SmartArray<T>& SmartArray<T>::operator=(SmartArray &&other) noexcept {

        if(&other == this) return *this;

        decref();

        Allocator<T*>::release(buffer);

        this->buffer = other.buffer;
        this->counter = other.counter;
        this->arraySize = other.arraySize;
        this->arrayCapacity = other.arrayCapacity;

        other.buffer = nullptr;
        other.counter = nullptr;
        other.arraySize = 0;
        other.arrayCapacity = 0;

        return *this;

    }

    template<class T>
    Collection<T *>::Iterator SmartArray<T>::begin() const {
        return typename Super::Iterator(buffer);
    }

    template<class T>
    Collection<T *>::Iterator SmartArray<T>::end() const {
        return typename Super::Iterator(buffer + size());
    }

    template<class T>
    size_t SmartArray<T>::size() const {
        return this->arraySize;
    }

    template<class T>
    size_t SmartArray<T>::capacity() const {
        return this->arrayCapacity;
    }

    template<class T>
    T *&SmartArray<T>::get(size_t index) const {
        nassertf(index < arraySize, String::format("SmartArray: Index {} out of bounds ({})", index, arraySize));
        return buffer[index];
    }

    template<class T>
    auto SmartArray<T>::add(T *const &element) -> decltype(*this) & {

        if (!element) return *this;

        extend(1);
        buffer[arraySize++] = element;
        return *this;

    }

    template<class T>
    auto SmartArray<T>::addAll(const Collection<T *> &array) -> decltype(*this) & {

        extend(array.size());

        for(T* element : array){
            buffer[arraySize++] = element;
        }

        return *this;

    }

    template<class T>
    auto SmartArray<T>::insert(T *const &element, size_t index) -> decltype(*this) & {

        if (!element) return *this;

        if (index >= arraySize) {
            return add(element);
        }

        extend(1);

        Allocator<T*>::move(buffer + index, buffer + arraySize, buffer + index + 1);

        buffer[index] = element;
        ++arraySize;

        return *this;

    }

    template<class T>
    auto SmartArray<T>::insertAll(const Collection<T *> &array, size_t index) -> decltype(*this) & {

        if (index >= arraySize) {
            return addAll(array);
        }

        extend(array.size());

        Allocator<T*>::move(buffer + index, buffer + arraySize, buffer + index + array.size() + 1);

        for(T* element : array){

            buffer[index++] = element;
            ++arraySize;

        }

        return *this;

    }

    template<class T>
    bool SmartArray<T>::removeAt(size_t index) {

        if (index > size()) return false;

        Allocator<T>::destroy(buffer[index]);

        Allocator<T*>::move(buffer + index + 1, buffer + arraySize, buffer + index);

        --arraySize;

        return true;

    }

    template<class T>
    bool SmartArray<T>::removeAllOf(T *const &element) {

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
    bool SmartArray<T>::contains(T *const &element) const {

        for (size_t i = 0; i < arraySize; ++i) {
            if (buffer[i] == element) return true;
        }

        return false;

    }

    template<class T>
    void SmartArray<T>::clear() {

        for (size_t i = 0; i < arraySize; ++i) {
            Allocator<T>::destroy(buffer[i]);
        }

        Allocator<T*>::release(buffer);

        this->arraySize = 0;
        this->arrayCapacity = 0;

    }

    template<class T>
    bool SmartArray<T>::isEmpty() const {
        return arraySize == 0;
    }

}

#endif