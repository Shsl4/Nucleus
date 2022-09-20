#pragma once

#include "CoreMacros.h"
#include "AllocationObserver.h"

namespace TinyCore {

	template<typename T>
	class Allocator {

	public:

		static T* allocate(size_t size) {

			if(size == 0) return nullptr;

			AllocationObserver::statAlloc<T>(size);

			return new T[size];

		}

		static void release(T*& ptr){

			if(!ptr) return;

			AllocationObserver::statRelease<T>();
			
			delete[] ptr;

			ptr = nullptr;

		}

		template<typename ... Args>
		static T* construct(Args&&... args){

			AllocationObserver::statAlloc<T>(1);
			return new T(args...);

		}

		static void destroy(T*& ptr) {

			if(!ptr) return;

			delete ptr;

			AllocationObserver::statRelease<T>();
			
			ptr = nullptr;

		}

		static void reallocate(T*& ptr, size_t oldSize, size_t newSize) {

			if(!ptr || oldSize == newSize) return;

			if(newSize == 0) { release(ptr); return; }

			T* newPtr = allocate(newSize);
			size_t copySize = oldSize > newSize ? newSize : oldSize;

			move(ptr, ptr + copySize, newPtr);

			release(ptr);
			ptr = newPtr;

		}

		static FORCEINLINE void moveBack(T* from, T* to, T* dest){

			while(from != to) { *dest++ = std::move(*from++); }

		}

		static FORCEINLINE void moveForward(T* from, T* to, T* dest){

			dest += to - from - 1;
			while(from != to) {	*dest-- = std::move(*(to-- - 1)); }

		}

		static FORCEINLINE void move(T* from, T* to, T* dest){

			assert(from && to && dest);
			if(dest > from) { moveForward(from, to, dest); return; }
			moveBack(from, to, dest);

		}

		static FORCEINLINE void copyBack(T* from, T* to, T* dest){

			while(from != to) { *dest++ = *from++; }

		}

		static FORCEINLINE void copyForward(T* from, T* to, T* dest){

			dest += to - from - 1;
			while(from != to) {	*dest-- = *(to-- - 1); }

		}

		static FORCEINLINE void copy(T* from, T* to, T* dest){

			assert(from && to && dest);
			if(dest > from) { moveForward(from, to, dest); return; }
			moveBack(from, to, dest);

		}

	};

}
