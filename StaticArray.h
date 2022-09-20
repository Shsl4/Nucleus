#pragma once

#include "Allocator.h"

namespace TinyCore {

	template<typename T, size_t size>
	class StaticArray {

	public:

		StaticArray() {
			this->buffer = Allocator<T>::allocate(size);
		}

		StaticArray(StaticArray<T, size> const& other){

			this->buffer = Allocator<T>::allocate(size);
			Allocator<T>::copy(other.buffer, other.buffer + size, this->buffer);
		
		}

		StaticArray(StaticArray<T, size>&& other){

			this->buffer = Allocator<T>::allocate(size);
			Allocator<T>::move(other.buffer, other.buffer + size, this->buffer);
		
		}

		~StaticArray() {

			Allocator<T>::release(buffer);

		}

		FORCEINLINE size_t getSize() const noexcept {
			return size;
		}

		T& operator[](size_t index){

			assert(index < size);
			return buffer[index];

		}

		T* begin() const {

			return buffer;

		}

		T* end() const {

			return buffer + size;

		}

	private:

		T* buffer = nullptr;

	};

}
