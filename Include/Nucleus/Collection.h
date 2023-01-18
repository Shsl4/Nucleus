#pragma once

#include <Nucleus/CoreMacros.h>

namespace Nucleus {

    template<class T>
    class Collection {

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

        Collection() = default;

        virtual ~Collection() = default;

        virtual Iterator begin() const = 0;

        virtual Iterator end() const = 0;

        NODISCARD virtual size_t size() const = 0;

        NODISCARD virtual size_t capacity() const = 0;

        NODISCARD virtual T& get(size_t index) const = 0;

        virtual auto add(T const& element) -> decltype(*this)& = 0;

        virtual auto addAll(Collection const& array) -> decltype(*this)& = 0;

        virtual auto insert(T const& element, size_t index) -> decltype(*this)& = 0;

        virtual auto insertAll(Collection const& array, size_t index) -> decltype(*this)& = 0;

        virtual bool removeAt(size_t index) = 0;

        virtual bool removeAllOf(T const& element) = 0;

        virtual bool contains(T const& element) const = 0;

        virtual void clear() = 0;

        NODISCARD virtual bool isEmpty() const = 0;

        void randomize();

        T& operator[](size_t index) const;

        friend Collection& operator+=(Collection& collection, T const& element){
            return collection.add(element);
        }

        friend Collection& operator+=(Collection& collection, Collection const& other){
            return collection.addAll(other);
        }

        friend Collection& operator+=(Collection& collection, std::initializer_list<T> const& list){

            for (T const& element : list) {
                collection.add(element);
            }

            return collection;

        }

        friend Collection& operator-=(Collection& collection, std::initializer_list<T> const& list){

            for (T const& element : list) {
                collection.removeAllOf(element);
            }

            return collection;

        }

    };

}

#define ARRAY_INLINE
#include <Nucleus/Inline/Collection.inl>
#undef ARRAY_INLINE