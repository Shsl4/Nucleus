#pragma once

#include "CoreMacros.h"
#include "Allocator.h"
#include "MutableArray.h"
#include "Exceptions.h"
#include <iterator>
#include <stdexcept>

namespace TinyCore {

    template <typename KeyType, typename ValueType>
    class Map {
        
        class Entry {

        public:

            Entry() = default;

            Entry(KeyType const& t, ValueType const& y) : key(t), value(y) {

            }

            Entry(KeyType&& t, ValueType&& y) : key(std::move(t)), value(std::move(y)) {

            }

            NODISCARD FORCEINLINE KeyType const& getKey() const { return key; }

            NODISCARD FORCEINLINE ValueType const& getValue() const { return value; }

            NODISCARD FORCEINLINE ValueType& getValue() { return value; }

        private:

            friend Map;
            
            KeyType key{};
            ValueType value{};

        };
    
    public:

        class Iterator {

        public:

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = Entry;
            using pointer           = Entry*;
            using reference         = Entry&;

            Iterator(pointer ptr) : ptr(ptr){

            }

            reference operator*() const {
                return *ptr;
            }

            pointer operator->() {
                return ptr;
            }

            pointer get() {
                return ptr;
            }

            reference operator++() { ++ptr; return *ptr; }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }

        private:

            pointer ptr;

        };

        Map() = default;

        ~Map() {

            this->count = 0;
            this->capacity = 0;
            Allocator<Entry>::release(buffer);

        }

        explicit Map(size_t cap) : capacity(cap){
            this->buffer = Allocator<Entry>::allocate(capacity);
        }

        Map(std::initializer_list<Entry> list){

            this->capacity = list.size();
            this->count = list.size();
            this->buffer = Allocator<Entry>::allocate(capacity);
            Allocator<Entry>::copy(list.begin(), list.end(), buffer);

        }

        Map(Map const& other) {
            *this = other;
        }

        Map(Map&& other) noexcept {
            *this = std::move(other);
        }

        ValueType& operator[](KeyType const& key) {

            for(auto& p: *this){
                if(p.key == key){
                    return p.getValue();
                }
            }

            throw BadKeyException("The provided key does not exist in the map.");
            
        }
        
        bool operator+=(Entry const& element) {
            return add(element);
        }

        bool operator+=(Entry&& element) {
            return add(std::move(element));
        }

        Map& operator=(Map const& other) {
            
            if(&other == this) return *this;

            const Entry* start = other.begin().get();
            
            Allocator<Entry>::reallocate(buffer, capacity, other.capacity);
            Allocator<Entry>::copy(start, start + other.capacity, buffer);

            this->count = other.count;
            this->capacity = other.capacity;
            
            return *this;
            
        }

        Map& operator=(Map&& other) noexcept {

            if(&other == this) return *this;

            Allocator<Entry>::release(buffer);

            this->buffer = other.buffer;
            this->count = other.count;
            this->capacity = other.capacity;

            other.buffer = nullptr;
            other.count = 0;
            other.capacity = 0;
            
            return *this;
            
        }

        bool add(KeyType const& key, ValueType const& value){
            return add({ key, value });
        }

        bool add(KeyType&& key, ValueType&& value){
            return add({ std::move(key), std::move(value) });
        }

        bool removeByKey(KeyType const& key) {

            for(size_t i = 0; i < count; ++i){

                if(buffer[i].key == key) {

                    Allocator<Entry>::move(buffer + i + 1, buffer + count, buffer + i);
                    --count;
                    return true;

                }

            }

            return false;

        }

        bool add(Entry const& element) {

            if(containsKey(element.key)) return false;

            checkSize();
            buffer[count] = element;
            ++count;

            return true;

        }

        bool getValueByKey(KeyType const& key, ValueType& out) const {

            for(auto const& p: *this){

                if(p.key == key){
                    out = p.getValue();
                    return true;
                }

            }

            return false;

        }

        bool add(Entry&& element) {

            if(containsKey(element.key)) return false;

            checkSize();
            buffer[count] = std::move(element);
            ++count;

            return true;

        }
        
        MutableArray<KeyType> getKeysByValue(ValueType const& value) const {

            MutableArray<KeyType> arr;

            for(auto const& p: *this){

                if(p.getValue() == value){
                    arr += p.key;
                }

            }

            return arr;

        }
        
        bool containsKey(KeyType const& key) const {

            for(Entry const& p : *this){

                if(key == p.key) {

                    return true;

                }

            }

            return false;

        }

        MutableArray<KeyType> getKeys() const {

            MutableArray<KeyType> arr;

            for(auto const& p: *this){
                arr += p.key;
            }

            return arr;

        }

        MutableArray<ValueType> getValues() const {

            MutableArray<ValueType> arr;

            for(auto const& p: *this){
                arr += p.getValue();
            }

            return arr;

        }

        FORCEINLINE size_t getCount() const { return count; }

        FORCEINLINE size_t getCapacity() const { return capacity; }

        Iterator begin() const { return Iterator(buffer); }
        Iterator end() const { return Iterator(buffer + count); }
        
    private:

        void checkSize(size_t size = 1){

            if(count + size > capacity){

                const size_t newSize = capacity > 0 ? capacity * 2 : 5;
                Allocator<Entry>::reallocate(buffer, capacity, newSize);
                capacity = newSize;

            }

        }

        Entry* buffer = nullptr;
        size_t count = 0;
        size_t capacity = 0;

    };

}
