#pragma once

#include <Nucleus/Collection.h>

namespace Nucleus {

    template<typename T>
    class List : public Collection<T> {

    public:

        class Node {

            friend List;

        public:

            static Node* make(T const& element) {
                Node* n = Allocator<Node>::construct();
                n->value = element;
                return n;
            }

            ~Node() {
                Allocator<Node>::destroy(next);
            }

            Node& operator=(T const& other){
                this->value = other;
                return *this;
            }

        private:

            T value;
            Node* next;

        };

        class NodeIterator : public Collection<T>::IteratorBase {

            using reference = T&;
            using pointer = T*;

        public:

            pointer operator->() override {
                return get();
            }

            pointer get() const override {
                return &node->value;
            }

            reference operator*() override {
                return node->value;
            }

            explicit NodeIterator(Node* node) : node(node) { }

            reference operator++() override {
                reference ref = this->node->value;
                this->node = this->node->next;
                return ref;

            }

        protected:

            NODISCARD bool equal(typename Collection<T>::IteratorBase const &other) const override {
                return static_cast<NodeIterator const&>(other).node == node;
            }

        private:

            Node* node;

        };
        
        List(std::initializer_list<T> list){
            
            for (auto const& e : list) {
                add(e);
            }
            
        }

        NODISCARD typename Collection<T>::Iterator begin() const override {
            return typename Collection<T>::Iterator(Allocator<NodeIterator>::construct(root));
        }

        NODISCARD typename Collection<T>::Iterator end() const override {
            return typename Collection<T>::Iterator(Allocator<NodeIterator>::construct(lastNode()));
        }

        NODISCARD size_t size() const override {
            return cachedSize;
        }

        NODISCARD size_t capacity() const override {
            return cachedSize;
        }

        T &get(size_t index) const override {

            Node* node = root;
            size_t i = 0;

            while(node && i < index){
                node = node->next;
                ++i;
            }

            nthrowif(!node, "List index out of range");

            return node->value;
        }

        auto add(const T &element) -> decltype(*this) & override {

            ++cachedSize;
            
            if(!root){ root = Node::make(element); return *this; }

            lastNode()->next = Node::make(element);
            
            return *this;

        }

        auto addAll(const Collection<T> &collection) -> decltype(*this) & override {
            
            for(auto const& elem : collection){
                add(elem);
            }
            
            return *this;
            
        }

        auto insert(const T &element, size_t index) -> decltype(*this) & override {

            if(index >= cachedSize){
                add(element); return *this;
            }

            Node* node = root;
            Node* prev = nullptr;
            size_t i = 0;

            while(i < index){

                prev = node;
                node = node->next;
                ++i;

            }

            Node* newNode = Node::make(element);

            if(node == root){
                newNode->next = root;
                root = newNode;
            }
            else{
                newNode->next = prev->next;
                prev->next = newNode;
            }

            ++cachedSize;

            return *this;

        }

        auto insertAll(const Collection<T> &collection, size_t index) -> decltype(*this) & override {

            for(auto& elem : collection){
                insert(elem, index++);
            }
            
            return *this;
            
        }

        bool removeAt(size_t index) override {

            if(!root) return false;

            Node* node = root;
            Node* prev = nullptr;
            size_t i = 0;

            while(i < index){

                if(!node) return false;

                prev = node;
                node = node->next;
                ++i;

            }

            if(node){

                if (node == root) {
                    root = node->next;
                }
                else{
                    prev->next = node->next;
                }

                node->next = nullptr;
                Allocator<Node>::destroy(node);
                --cachedSize;
                return true;

            }

            return false;
        }
        
        void clear() override {

        }

        NODISCARD Node* lastNode() const{

            Node* node = root;

            while (node->next){
                node = node->next;
            }

            return node;

        }
        
        ~List() override {
            Allocator<Node>::destroy(root);
        }

    private:

        Node* root = nullptr;
        size_t cachedSize = 0;

    };

}
