#pragma once

#include <Nucleus/CoreMacros.h>

namespace Nucleus {

    /*!
     * An allocator utility class used to abstract away the usage of new and delete.
     * @tparam T The type to manage
     */
    template<typename T>
    class Allocator {
        
    public:

        /*!
         * Allocates an uninitialized buffer of size @see size
         * @param size The size of the buffer to allocate
         * @return The newly allocated buffer
         */
        static T* allocate(size_t size);

        /*!
         * Releases a buffer allocated with @see allocate
         * @param ptr The pointer to release
         */
        static void release(T*& ptr);

        /*!
         * Constructs a single object of type T by forwarding the provided arguments to the constructor.
         * @tparam Args A variadic argument template
         * @param args The arguments to pass
         * @return The newly allocated object
         */
        template<typename ... Args>
        static T* construct(Args&&... args);

        /*!
         * Destroys a single object allocated with @see construct
         * @param ptr The pointer to release
         */
        static void destroy(T*& ptr);

        /*!
         * Reallocates the pointer to a new buffer and assigns it to @see ptr
         * @param ptr A reference to the buffer containing the data to reallocate
         * @param oldSize The current size of the buffer
         * @param newSize The new size of the buffer
         */
        static void reallocate(T*& ptr, size_t oldSize, size_t newSize);

        /*!
         * Moves the data from back to front
         * @param from The head of the buffer
         * @param to The tail of the buffer
         * @param dest The move destination
         */
        static FORCEINLINE void moveBack(T* from, T* to, T* dest){
            while(from != to) { *dest++ = std::move(*from++); }
        }

        /*!
         * Moves data from front to back
         * @param from The head of the buffer
         * @param to The tail of the buffer
         * @param dest The move destination
         */
        static FORCEINLINE void moveForward(T* from, T* to, T* dest){
            
            dest += to - from - 1;
            while(from != to) {	*dest-- = std::move(*(to-- - 1)); }
            
        }

        /*!
         * Automatically selects the most appropriate move function (@see moveForward or @see moveBack)
         * @param from The head of the buffer
         * @param to The tail of the buffer
         * @param dest The move destination
         */
        static void move(T* from, T* to, T* dest){
            
            assert(from && to && dest);
            if(dest > from) { moveForward(from, to, dest); return; }
            moveBack(from, to, dest);
            
        }

        /*!
         * Swaps the contents of @see a and @see b using move if possible
         * @param a The first element
         * @param b The second element
         */
        static FORCEINLINE void swap(T& a, T& b){

            T storage = std::move(a);
            a = std::move(b);
            b = std::move(storage);

        }

        /*!
         * Copies the data from back to front
         * @param from The head of the buffer
         * @param to The tail of the buffer
         * @param dest The move destination
         */
        static FORCEINLINE void copyBack(const T* from, const T* to, T* dest){
            while(from != to) { *dest++ = *from++; }
        }


        /*!
         * Copies data from front to back
         * @param from The head of the buffer
         * @param to The tail of the buffer
         * @param dest The move destination
         */
        static FORCEINLINE void copyForward(const T* from, const T* to, T* dest){
            
            dest += to - from - 1;
            while(from != to) {	*dest-- = *(to-- - 1); }
            
        }

        /*!
         * Automatically selects the most appropriate copy function (@see copyForward or @see copyBack)
         * @param from The head of the buffer
         * @param to The tail of the buffer
         * @param dest The move destination
         */
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
