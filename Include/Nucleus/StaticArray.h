#pragma once

#include <Nucleus/Collection.h>
#include <Nucleus/Exceptions.h>

namespace Nucleus {
    
    template<typename T, size_t sz>
    class StaticArray : public Collection<T> {

        using Super = Collection<T>;

    public:
        
        StaticArray() = default;

        StaticArray(std::initializer_list<T> const& list){

            if(list.size() > sz){
                throw Exceptions::Exception("Initializer list size exceeds maximal array size");
            }
            
            Allocator<T>::copy(list.begin(), list.end(), this->buffer);

        }

        StaticArray(StaticArray<T, sz> const& other){

            Allocator<T>::copy(other.buffer, other.buffer + sz, this->buffer);

        }

        StaticArray(StaticArray<T, sz>&& other) noexcept {
            
            Allocator<T>::move(other.buffer, other.buffer + sz, this->buffer);
            
        }

        StaticArray<T, sz>& operator=(StaticArray<T, sz> const& other) {

            if(&other == this) return *this;

            Allocator<T>::copy(other.begin(), other.begin() + sz, buffer);

            return *this;

        }

        StaticArray<T, sz>& operator=(StaticArray<T, sz>&& other) noexcept {

            if(&other == this) return *this;

            Allocator<T>::move(other.begin(), other.begin() + sz, buffer);

            return *this;

        }
        
        ~StaticArray() = default;

        NODISCARD size_t size() const override { return sz; }

        NODISCARD size_t capacity() const override { return sz; }

        NODISCARD bool isEmpty() const override { return false; }

        NODISCARD T &get(size_t index) const override { assert(index < sz); return *(const_cast<T*>(buffer) + index); }

        auto add(const T&) -> decltype(*this) & override {
            throw Exceptions::UnsupportedOperation("Unsupported operation add() on StaticArray");
        }

        auto addAll(const Collection<T>&) -> decltype(*this) & override {
            throw Exceptions::UnsupportedOperation("Unsupported operation addAll() on StaticArray");
        }

        auto insert(const T&, size_t) -> decltype(*this) & override {
            throw Exceptions::UnsupportedOperation("Unsupported operation insert() on StaticArray");
        }

        auto insertAll(const Collection<T>&, size_t) -> decltype(*this) & override {
            throw Exceptions::UnsupportedOperation("Unsupported operation insertAll() on StaticArray");
        }

        bool remove(const T&) override { return false; }

        bool removeAt(size_t) override { return false; }

        auto removeAll(const Collection<T>&) -> decltype(*this) & override { return *this; }

        bool contains(const T &element) const override {

            for(size_t i = 0; i < sz; ++i){

                if (buffer[i] == element){
                    return true;
                }

            }

            return false;

        }

        void clear() override { }

        typename Super::Iterator begin() const override {
            const T* b = buffer;
            return typename Super::Iterator(Allocator<typename Super::ContinuousIterator>::construct(const_cast<T*>(b)));
        }

        typename Super::Iterator end() const override {
            const T* e = buffer + sz;
            return typename Super::Iterator(Allocator<typename Super::ContinuousIterator>::construct(const_cast<T*>(e)));
        }

    private:
        
        T buffer[sz];
        
    };
    
}
