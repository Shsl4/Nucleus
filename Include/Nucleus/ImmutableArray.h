#pragma once

#include <Nucleus/Allocator.h>

namespace Nucleus {

	template<typename T>
	class MutableArray;
	
	template<typename T>
	class ImmutableArray {

	public:

		ImmutableArray();

		explicit ImmutableArray(size_t cap);

		ImmutableArray(T* buf, size_t size);

        ImmutableArray(std::initializer_list<T> const& list);

		ImmutableArray(ImmutableArray<T> const& other);

		ImmutableArray(ImmutableArray<T>&& other) noexcept;

		ImmutableArray& operator=(ImmutableArray<T> const& other);

		ImmutableArray& operator=(ImmutableArray<T>&& other) noexcept;

		MutableArray<T> toMutable() const;

		~ImmutableArray();

		FORCEINLINE size_t getSize() const noexcept;

		T& operator[](size_t index);

		T* begin() const;

		T* end() const;

	private:

		size_t capacity = 0;
		T* buffer = nullptr;

	};
	
}
#define IMMUTABLE_INLINE
#include "Inline/ImmutableArray.inl"
#undef IMMUTABLE_INLINE
