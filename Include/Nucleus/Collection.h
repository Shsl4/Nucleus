#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/Allocator.h>

namespace Nucleus {

    template<class T>
    class Collection {

    public:
        
        class IteratorBase {
        
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T*;
            using reference         = T&;
            
        public:
            
            virtual pointer operator->() = 0;
            
            virtual pointer get() const = 0;
            
            virtual reference operator*() = 0;
        
            virtual reference operator++() = 0;
            
            friend bool operator==(const IteratorBase& a, const IteratorBase& b) {
                return typeid(a) == typeid(b) && a.equal(b);
            }

            virtual ~IteratorBase() = default;

        protected:
        
            virtual bool equal(const IteratorBase& other) const { return false; }
            
        };
        
        class ContinuousIterator : public IteratorBase {

            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T*;
            using reference         = T&;

        public:

            explicit ContinuousIterator(T* ptr) : ptr(ptr){

            }

            pointer operator->() override {
                return ptr;
            }

            pointer get() const override {
                return ptr;
            }

            reference operator*() override {
                return *ptr;
            }

            reference operator++() override {
                return *(++ptr);
            }

        protected:

            bool equal(const IteratorBase &other) const override {
                return static_cast<const ContinuousIterator&>(other).ptr == ptr;
            }

        private:

            T* ptr;

        };

        class Iterator {

        public:

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T*;
            using reference         = T&;

            explicit Iterator(IteratorBase* it);

            reference operator*() const;

            pointer operator->();

            virtual reference operator++();

            pointer get();

            virtual ~Iterator() {
                Allocator<IteratorBase>::destroy(it);
            }

            friend bool operator==(const Iterator& a, const Iterator& b) { return *a.it == *b.it; }

        protected:

            IteratorBase* it;

        };

        Collection() = default;

        virtual ~Collection() = default;

        virtual Iterator begin() const = 0;

        virtual Iterator end() const = 0;

        NODISCARD virtual size_t size() const = 0;

        NODISCARD virtual size_t capacity() const = 0;

        NODISCARD virtual T& get(size_t index) const = 0;

        virtual auto add(T const& element) -> decltype(*this)& = 0;

        virtual auto addAll(Collection const& collection) -> decltype(*this)& = 0;

        virtual auto insert(T const& element, size_t index) -> decltype(*this)& = 0;

        virtual auto insertAll(Collection const& collection, size_t index) -> decltype(*this)& = 0;

        virtual bool remove(T const& element) = 0;

        virtual bool removeAt(size_t index) = 0;

        virtual auto removeAll(Collection const& collection) -> decltype(*this)& = 0;

        virtual bool contains(T const& element) const = 0;

        virtual void clear() = 0;

        NODISCARD virtual bool isEmpty() const = 0;

        void randomize();

        T& operator[](size_t index) const;

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator+(CollectionType& collection, T const& element) -> CollectionType {

            CollectionType copy = collection;
            copy.add(element);
            return copy;

        }

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator-(CollectionType& collection, T const& element) -> CollectionType {

            CollectionType copy = collection;
            copy.remove(element);
            return copy;

        }

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator+=(CollectionType& collection, T const& element) -> CollectionType& {
            return collection.add(element);
        }

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator-=(CollectionType& collection, T const& element) -> CollectionType& {
            collection.remove(element);
            return collection;
        }

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator+=(CollectionType& collection, Collection const& other) -> CollectionType& {
            return collection.addAll(other);
        }

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator-=(CollectionType& collection, Collection const& other) -> CollectionType& {
            return collection.removeAll(other);
        }

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator+=(CollectionType& collection, std::initializer_list<T> const& list) -> CollectionType& {

            for (T const& element : list) {
                collection.add(element);
            }

            return collection;

        }

        template<typename CollectionType> requires (std::is_base_of_v<Collection, CollectionType>)
        friend auto operator-=(CollectionType& collection, std::initializer_list<T> const& list) -> CollectionType& {

            for (T const& element : list) {
                collection.remove(element);
            }

            return collection;

        }

        friend bool operator==(Collection const& a, Collection const& b) {

            if(a.size() != b.size()) { return false; }

            for (size_t i = 0; i < a.size(); ++i) {
                if(a[i] != b[i]){
                    return false;
                }
            }

            return true;

        }


    };

}

#define COLLECTION_INLINE
#include <Nucleus/Inline/Collection.inl>
#undef COLLECTION_INLINE
