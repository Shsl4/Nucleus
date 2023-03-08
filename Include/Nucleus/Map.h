#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/Allocator.h>
#include <Nucleus/MutableArray.h>
#include <Nucleus/Exceptions.h>
#include <iterator>

namespace Nucleus {

    template <typename KeyType, typename ValueType>
    class Map {

    public:
        
        class Entry {

        public:

            Entry() = default;

            Entry(KeyType const& t, ValueType const& y) : keyObject(t), valueObject(y) {

            }

            Entry(KeyType&& t, ValueType&& y) : keyObject(std::move(t)), valueObject(std::move(y)) {

            }

            NODISCARD FORCEINLINE KeyType const& key() const { return keyObject; }

            NODISCARD FORCEINLINE ValueType const& value() const { return valueObject; }

            NODISCARD FORCEINLINE ValueType& value() { return valueObject; }

        private:

            friend Map;
            
            KeyType keyObject {};
            ValueType valueObject {};

        };
        
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

            reference operator++() { return *++ptr; }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }

        private:

            pointer ptr;

        };

        Map() = default;

        ~Map() {

            reset();

        }

        explicit Map(size_t cap) : storage(cap){
            this->buffer = Allocator<Entry>::allocate(storage);
        }

        Map(std::initializer_list<Entry> list){

            this->storage = list.size();
            this->count = list.size();
            this->buffer = Allocator<Entry>::allocate(storage);
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
                if(p.key() == key){
                    return p.value();
                }
            }

            throw Exceptions::BadKey("The provided key does not exist in the map.");
            
        }

        ValueType& operator[](KeyType const& key) const {

            for(auto& p: *this){
                if(p.key() == key){
                    return p.value();
                }
            }

            throw Exceptions::BadKey("The provided key does not exist in the map.");

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
            
            Allocator<Entry>::reallocate(buffer, storage, other.storage);
            Allocator<Entry>::copy(start, start + other.storage, buffer);

            this->count = other.count;
            this->storage = other.storage;
            
            return *this;
            
        }

        Map& operator=(Map&& other) noexcept {

            if(&other == this) return *this;

            Allocator<Entry>::release(buffer);

            this->buffer = other.buffer;
            this->count = other.count;
            this->storage = other.storage;

            other.buffer = nullptr;
            other.count = 0;
            other.storage = 0;
            
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

                if(buffer[i].key() == key) {

                    Allocator<Entry>::move(buffer + i + 1, buffer + count, buffer + i);
                    --count;
                    return true;

                }

            }

            return false;

        }

        bool add(Entry const& element) {

            if(containsKey(element.key())) return false;

            checkSize();
            buffer[count++] = element;

            return true;

        }

        void reset() {

            this->count = 0;
            this->storage = 0;
            Allocator<Entry>::release(buffer);

        }

        bool valueByKey(KeyType const& key, ValueType& out) const {

            for(auto const& p: *this){

                if(p.key() == key){
                    out = p.value();
                    return true;
                }

            }

            return false;

        }

        bool add(Entry&& element) {

            if(containsKey(element.key())) return false;

            checkSize();
            buffer[count++] = std::move(element);

            return true;

        }

        NODISCARD FORCEINLINE Entry& get(size_t index) const {

            assert(index < count);
            return buffer[index];
            
        }
        
        NODISCARD MutableArray<KeyType> keysByValue(ValueType const& value) const {

            MutableArray<KeyType> arr;

            for(auto const& p: *this){

                if(p.value() == value){
                    arr += p.key();
                }

            }

            return arr;

        }
        
        NODISCARD bool containsKey(KeyType const& key) const {

            for(Entry const& p : *this){

                if(key == p.key()) {

                    return true;

                }

            }

            return false;

        }

        NODISCARD MutableArray<KeyType> keys() const {

            MutableArray<KeyType> arr;

            for(auto const& p: *this){
                arr += p.key();
            }

            return arr;

        }

        NODISCARD MutableArray<ValueType> values() const {

            MutableArray<ValueType> arr;

            for(auto const& p: *this){
                arr += p.value();
            }

            return arr;

        }

        NODISCARD FORCEINLINE size_t size() const { return count; }

        NODISCARD FORCEINLINE size_t capacity() const { return storage; }

        NODISCARD Iterator begin() const { return Iterator(buffer); }
        
        NODISCARD Iterator end() const { return Iterator(buffer + count); }
        
    private:

        void checkSize(size_t size = 1){

            if(count + size > storage){

                const size_t newSize = storage > 0 ? storage * 2 : 5;
                Allocator<Entry>::reallocate(buffer, storage, newSize);
                storage = newSize;

            }

        }

        Entry* buffer = nullptr;
        size_t count = 0;
        size_t storage = 0;

    };

}
