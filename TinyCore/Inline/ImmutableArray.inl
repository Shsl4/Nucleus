#pragma once

#ifdef IMMUTABLE_INLINE

#include "../ImmutableArray.h"

namespace TinyCore {

    template <typename T>
	ImmutableArray<T>::ImmutableArray() = default;

	template <typename T>
	ImmutableArray<T>::ImmutableArray(size_t cap): capacity(cap) {

		assert(capacity > 0);
		this->buffer = Allocator<T>::allocate(capacity);

	}

	template <typename T>
	ImmutableArray<T>::ImmutableArray(T* buf, size_t size) : capacity(buf ? size : 0) {

		if(!buf) { return; }

		this->buffer = Allocator<T>::allocate(size);

		Allocator<T>::copy(buf, buf + size, this->buffer);

	}

	template <typename T>
	ImmutableArray<T>::ImmutableArray(std::initializer_list<T> const& list) {

		this->capacity = list.size();
		this->buffer = Allocator<T>::allocate(capacity);
		Allocator<T>::copy(list.begin(), list.end(), buffer);

	}

	template <typename T>
	ImmutableArray<T>::ImmutableArray(ImmutableArray<T> const& other) {
		*this = other;
	}

	template <typename T>
	ImmutableArray<T>::ImmutableArray(ImmutableArray<T>&& other) noexcept {
		*this = std::move(other);
	}

	template <typename T>
	ImmutableArray<T>& ImmutableArray<T>::operator=(ImmutableArray<T> const& other) {

		if(&other == this) return *this;

		Allocator<T>::release(this->buffer);

		const T* start = other.begin();

		this->capacity = other.capacity;
		this->buffer = Allocator<T>::allocate(capacity);
			
		Allocator<T>::copy(start, start + other.capacity, buffer);

		return *this;

	}

	template <typename T>
	ImmutableArray<T>& ImmutableArray<T>::operator=(ImmutableArray<T>&& other) noexcept {

		if(&other == this) return *this;

		Allocator<T>::release(this->buffer);

		T* start = other.begin();

		this->capacity = other.capacity;
		this->buffer = Allocator<T>::allocate(capacity);
			
		Allocator<T>::move(start, start + capacity, buffer);

		return *this;

	}

	template <typename T>
	MutableArray<T> ImmutableArray<T>::toMutable() const {
		return MutableArray<T>(buffer, capacity);
	}

	template <typename T>
	ImmutableArray<T>::~ImmutableArray() {

		Allocator<T>::release(buffer);

	}

	template <typename T>
	size_t ImmutableArray<T>::getSize() const noexcept {
		return capacity;
	}

	template <typename T>
	T& ImmutableArray<T>::operator[](size_t index) {

		assert(index < capacity);
		return buffer[index];

	}

	template <typename T>
	T* ImmutableArray<T>::begin() const {

		return buffer;

	}

	template <typename T>
	T* ImmutableArray<T>::end() const {

		return buffer + capacity;

	}
    
}

#endif