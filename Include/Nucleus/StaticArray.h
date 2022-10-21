#pragma once

#include <Nucleus/Allocator.h>

namespace Nucleus {
    
    template<typename T, size_t size>
    class StaticArray {
        
    public:
        
        StaticArray() = default;

        StaticArray(StaticArray<T, size> const& other){

            Allocator<T>::copy(other.buffer, other.buffer + size, this->buffer);

        }

        StaticArray(StaticArray<T, size>&& other) noexcept {
            
            Allocator<T>::move(other.buffer, other.buffer + size, this->buffer);
            
        }

        StaticArray<T, size>& operator=(StaticArray<T, size> const& other) {

            if(&other == this) return *this;

            Allocator<T>::copy(other.begin(), other.begin() + size, buffer);

            return *this;

        }

        StaticArray<T, size>& operator=(StaticArray<T, size>&& other) noexcept {

            if(&other == this) return *this;

            Allocator<T>::move(other.begin(), other.begin() + size, buffer);

            return *this;

        }
        
        ~StaticArray() = default;
        
        FORCEINLINE static size_t getSize() noexcept { return size; }
        
        T& operator[](size_t index){
            
            assert(index < size);
            return buffer[index];
            
        }
        
        T* begin() const { return const_cast<T*>(buffer); }
        
        T* end() const { return const_cast<T*>(buffer + size); }
        
    private:
        
        T buffer[size];
        
    };
    
}
