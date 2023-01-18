#pragma once

#include <Nucleus/CoreMacros.h>

namespace Nucleus {
    
    template<typename T>
    class Allocator {
        
    public:
        
        static T* allocate(size_t size);

        static void release(T*& ptr);

        template<typename ... Args>
        static T* construct(Args&&... args);

        static void destroy(T*& ptr);

        static void reallocate(T*& ptr, size_t oldSize, size_t newSize);

        static FORCEINLINE void moveBack(T* from, T* to, T* dest){
            
            while(from != to) { *dest++ = std::move(*from++); }
            
        }
        
        static FORCEINLINE void moveForward(T* from, T* to, T* dest){
            
            dest += to - from - 1;
            while(from != to) {	*dest-- = std::move(*(to-- - 1)); }
            
        }
        
        static void move(T* from, T* to, T* dest){
            
            assert(from && to && dest);
            if(dest > from) { moveForward(from, to, dest); return; }
            moveBack(from, to, dest);
            
        }

        static FORCEINLINE void swap(T& a, T& b){

            T storage = std::move(a);
            a = std::move(b);
            b = std::move(storage);

        }
        
        static FORCEINLINE void copyBack(const T* from, const T* to, T* dest){
            
            while(from != to) { *dest++ = *from++; }
            
        }
        
        static FORCEINLINE void copyForward(const T* from, const T* to, T* dest){
            
            dest += to - from - 1;
            while(from != to) {	*dest-- = *(to-- - 1); }
            
        }
        
        static void copy(const T* from, const T* to, T* dest){

            assert(from && to && dest);
            if(dest > from) { copyForward(from, to, dest); return; }
            copyBack(from, to, dest);
            
        }
        
    };
    
}

#define ALLOCATOR_INLINE
#include <Nucleus/Inline/Allocator.inl>
#undef ALLOCATOR_INLINE
