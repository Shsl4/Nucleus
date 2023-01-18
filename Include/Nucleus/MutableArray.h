#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/Allocator.h>
#include <iterator>

namespace Nucleus {

    template<typename T>
    class ImmutableArray;
    
    template<typename T>
    class MutableArray {
    
    public:

        class Iterator {

        public:

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T*;
            using reference         = T&;

            explicit Iterator(pointer ptr);

            reference operator*() const;

            pointer operator->();

            reference operator++();

            pointer get();

            friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
            friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }

        private:

            pointer ptr;

        };

        MutableArray() = default;

        virtual ~MutableArray();

        explicit MutableArray(size_t cap);

        MutableArray(std::initializer_list<T> list);

        MutableArray(T* buf, size_t size);

        MutableArray(MutableArray const& other);

        MutableArray(MutableArray&& other) noexcept;

        T& operator[](size_t index);

        T& operator[](size_t index) const;

        virtual MutableArray& operator+=(T const& element);

        virtual MutableArray& operator+=(MutableArray const& array);

        virtual MutableArray& operator+=(T&& element);

        MutableArray& operator=(MutableArray const& other);

        MutableArray& operator=(MutableArray&& other) noexcept;

        MutableArray& append(T const& element);

        MutableArray& append(T&& element);

        MutableArray& append(MutableArray const& array);

        MutableArray& insert(T const& element, size_t index);

        MutableArray& insert(T&& element, size_t index);

        MutableArray& insert(MutableArray const& array, size_t index);

        ImmutableArray<T> toImmutable() const;
        
        bool isEmpty() const { return this->count == 0; }

        void reset();

        bool removeAt(size_t index);

        bool removeAllOf(T const& value);

        virtual FORCEINLINE size_t size() const { return count; }

        FORCEINLINE size_t getCapacity() const { return capacity; }

        Iterator begin() const;

        Iterator end() const;

    protected:

        void checkSize(size_t size = 1);

        T* buffer = nullptr;
        size_t count = 0;
        size_t capacity = 0;

    };
    
}

#define MUTABLE_INLINE
#include <Nucleus/Inline/MutableArray.inl>
#undef MUTABLE_INLINE
