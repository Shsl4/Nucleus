#pragma once

#ifdef ALLOCATOR_INLINE

#include "../AllocationObserver.h"

namespace TinyCore {

    template <typename T>
    T* Allocator<T>::allocate(size_t size) {
            
        if(size == 0) return nullptr;
            
        AllocationObserver::statAlloc<T>(size);
            
        return new T[size];
            
    }

    template <typename T>
    void Allocator<T>::release(T*& ptr) {
            
        if(!ptr) return;
            
        AllocationObserver::statRelease<T>();
            
        delete[] ptr;
            
        ptr = nullptr;
            
    }

    template <typename T>
    template <typename ... Args>
    T* Allocator<T>::construct(Args&&... args) {
            
        AllocationObserver::statAlloc<T>(1);
        return new T(args...);
            
    }

    template <typename T>
    void Allocator<T>::destroy(T*& ptr) {
            
        if(!ptr) return;
            
        delete ptr;
            
        AllocationObserver::statRelease<T>();
            
        ptr = nullptr;
            
    }

    template <typename T>
    void Allocator<T>::reallocate(T*& ptr, size_t oldSize, size_t newSize) {

        if(!ptr) {
            ptr = allocate(newSize);
            return;
        }

        if(oldSize == newSize) return;
            
        if(newSize == 0) { release(ptr); return; }
            
        T* newPtr = allocate(newSize);
        size_t copySize = oldSize > newSize ? newSize : oldSize;
            
        move(ptr, ptr + copySize, newPtr);
            
        release(ptr);
        ptr = newPtr;
            
    }
    
}

#endif