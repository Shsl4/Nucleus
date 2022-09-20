#pragma once

#include "Allocator.h"

namespace TinyCore {

	template<typename T>
	class ImmutableArray {

	public:

		ImmutableArray() = delete;

		explicit ImmutableArray(size_t cap) : capacity(cap) {

			assert(capacity > 0);
			this->buffer = Allocator<T>::allocate(capacity);

		}

		ImmutableArray(T* buf, size_t size) : capacity(size) {

			assert(buf != nullptr && size > 0);

			this->buffer = Allocator<T>::allocate(size);

			Allocator<T>::copy(buf, buf + size, this->buffer);

		}

		ImmutableArray(ImmutableArray<T> const& other) {
			*this = other;
		}

		ImmutableArray(ImmutableArray<T>&& other) {
			*this = std::move(other);
		}

		ImmutableArray<T>& operator=(ImmutableArray<T> const& other) {

			if(&other == this) return *this;

			Allocator<T>::release(this->buffer);

			this->capacity = other.capacity;
			this->buffer = Allocator<T>::allocate(capacity);
			
			Allocator<T>::copy(other.begin(), other.begin() + capacity, buffer);

			return *this;

		}

		ImmutableArray<T>& operator=(ImmutableArray<T>&& other) {

			if(&other == this) return *this;

			Allocator<T>::release(this->buffer);

			this->capacity = other.capacity;
			this->buffer = Allocator<T>::allocate(capacity);
			
			Allocator<T>::move(other.begin(), other.begin() + capacity, buffer);

			return *this;

		}

		~ImmutableArray() {

			Allocator<T>::release(buffer);

		}

		FORCEINLINE size_t getSize() const noexcept {
			return capacity;
		}

		T& operator[](size_t index){

			assert(index < capacity);
			return buffer[index];

		}

		T* begin() const {

			return buffer;

		}

		T* end() const {

			return buffer + capacity;

		}

	private:

		size_t capacity = 0;
		T* buffer = nullptr;

	};

}
