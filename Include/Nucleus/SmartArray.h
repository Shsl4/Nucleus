#pragma once

#include <Nucleus/Collection.h>
#include <utility>

namespace Nucleus {

    template<class T>
    class SmartArray : public Collection<T*> {

        using Super = Collection<T*>;

    public:

        SmartArray() : arraySize(0), arrayCapacity(10), buffer(Allocator<T*>::allocate(arrayCapacity)),
                       counter(Allocator<RefCounter>::construct()) {

            this->counter->ref();

        }

        SmartArray(SmartArray const& other){
            *this = other;
        }

        SmartArray(SmartArray&& other) noexcept {
            *this = std::move(other);
        }

        ~SmartArray() override {
            decref();
        }

        SmartArray& operator=(SmartArray const& other){

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

        SmartArray& operator==(SmartArray&& other) noexcept{

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

        NODISCARD typename Super::Iterator begin() const override {
            return typename Super::Iterator(buffer);
        }

        NODISCARD typename Super::Iterator end() const override {
            return typename Super::Iterator(buffer + size());
        }

        NODISCARD size_t size() const override {
            return this->arraySize;
        }

        NODISCARD size_t capacity() const override {
            return this->arrayCapacity;
        }

        NODISCARD T *&get(size_t index) const override {
            nassertf(index < arraySize, String::format("SmartArray: Index {} out of bounds ({})", index, arraySize));
            return buffer[index];
        }

        auto add(T *const &element) -> decltype(*this)& override {

            if (!element) return *this;

            extend(1);
            buffer[arraySize++] = element;
            return *this;

        }

        auto addAll(const Collection<T *> &array) -> decltype(*this) & override {

            extend(array.size());

            for(T* element : array){
                buffer[arraySize++] = element;
            }

            return *this;
        }

        auto insert(T *const &element, size_t index) -> decltype(*this) & override {

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

        auto insertAll(const Collection<T *> &array, size_t index) -> decltype(*this) & override {

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

        bool removeAt(size_t index) override {
            return false;
        }

        bool removeAllOf(T *const &element) override {
            return false;
        }

        bool contains(T *const &element) override {

            for (size_t i = 0; i < arraySize; ++i) {
                if (buffer[i] == element) return true;
            }

            return false;

        }

        void clear() override {

            for (size_t i = 0; i < arraySize; ++i) {
                Allocator<T>::destroy(buffer[i]);
            }

            Allocator<T*>::release(buffer);

            this->arraySize = 0;
            this->arrayCapacity = 0;

        }

        bool isEmpty() override {
            return arraySize == 0;
        }

    private:

        void extend(size_t size) {

            if(arraySize + size > arrayCapacity){

                const size_t newSize = arrayCapacity > 0 ? arrayCapacity * 2 : 5;
                Allocator<T*>::reallocate(buffer, arrayCapacity, newSize);
                arrayCapacity = newSize;

            }

        }

        void decref(){

            if (!counter) return;

            counter->unref();

            if (counter->exhausted()){

                Allocator<RefCounter>::destroy(counter);
                clear();

            }

        }

        size_t arrayCapacity = 0;
        size_t arraySize = 0;
        T** buffer = nullptr;
        RefCounter* counter = nullptr;

    };

}