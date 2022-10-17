#pragma once

#include "Shared.h"

#ifdef SHARED_INLINE

namespace TinyCore {

    template <typename T>
    Shared<T>::Shared() = default;

    template <typename T>
    Shared<T>::Shared(nullptr_t) {
        
    }

    template <typename T>
    Shared<T>::~Shared() {
        drop();
    }

    template <typename T>
    template <typename ... Args>
    Shared<T> Shared<T>::make(Args&&... args) {

        Shared<T> shared;
        shared.counter = Allocator<RefCounter>::construct();
        shared.object = Allocator<T>::construct(std::forward<Args>(args)...);
        shared.counter->ref();
        return shared;
            
    }

    template <typename T>
    template <typename Y>
    Shared<T>::Shared(Shared<Y> const& other) {
        *this = other;
    }

    template <typename T>
    template <typename Y>
    Shared<T>::Shared(Shared<Y>&& other) {
        *this = std::move(other);
    }

    template <typename T>
    Shared<T>::Shared(Shared const& other) {
        *this = other;
    }

    template <typename T>
    Shared<T>::Shared(Shared&& other) noexcept {
        *this = std::move(other);
    }

    template <typename T>
    Shared<T>& Shared<T>::operator=(Shared const& other) noexcept {

        if(&other == this) return *this;

        if(other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;
            this->counter->ref();
                
        }
            
        return *this;
            
    }

    template <typename T>
    Shared<T>& Shared<T>::operator=(Shared&& other) noexcept {

        if(&other == this) return *this;

        if(other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;

            other.counter = nullptr;
            other.object = nullptr;
                
        }
            
        return *this;
            
    }

    template <typename T>
    Shared<T>& Shared<T>::operator=(nullptr_t) noexcept { drop(); return *this; }

    template <typename T>
    template <typename Y>
    Shared<T>& Shared<T>::operator=(Shared<Y> const& other) noexcept {

        static_assert(std::is_base_of_v<T, Y>, "Y must inherit T");
            
        if (other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;
            this->counter->ref();
                
        }

        return *this;
            
    }

    template <typename T>
    template <typename Y>
    Shared<T>& Shared<T>::operator=(Shared<Y>&& other) noexcept {

        static_assert(std::is_base_of_v<T, Y>, "Y must inherit T");
            
        if (other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;

            other.counter = nullptr;
            other.object = nullptr;
                
        }

        return *this;
            
    }
    
    template <typename T>
    String Shared<T>::getClass() const {
        return isValid() ? Type::getClassName(*object) : "nullptr";
    }

    template <typename T>
    void Shared<T>::drop() {
            
        if(counter) {

            counter->unref();
                
            if(!counter->valid()) {
                Allocator<T>::destroy(object);
            }

            if(counter->exhausted()) {
                Allocator<RefCounter>::destroy(counter);
            }
            
        }

        object = nullptr;
        counter = nullptr;
        
    }
    
    template <typename T>
    bool Shared<T>::operator==(Shared<T> const& other) const noexcept {
        return other.object == object && other.counter == counter;
    }

    template <typename T>
    template <typename Y>
    bool Shared<T>::operator==(Shared<Y> const& other) const noexcept {
        return other.object == object && other.counter == counter;
    }
    
    template <typename T>
    Weak<T> Shared<T>::makeWeak() const {
        
        if(!counter) return Weak<T>();

        Weak<T> weak;

        weak.counter = this->counter;
        weak.object = this->object;
        weak.counter->refWeak();
        
        return weak;
        
    }

    template<typename T>
    Shared<T>::operator bool() const {
        return isValid();
    }

    template<typename T>
    template<typename Y>
    bool Shared<T>::operator==(const Y &other) const noexcept {
        if(!isValid()) return false;
        return *object == other;
    }

    template <typename T>
    Weak<T>::~Weak() {

        if(counter) {

            counter->unrefWeak();

            if (counter->exhausted()) {
                Allocator<RefCounter>::destroy(counter);
            }
                
        }

        object = nullptr;
            
    }
    
    template <typename T>
    Weak<T>::Weak(Weak<T> const& other) {
        *this = other;
    }
    
    template <typename T>
    Weak<T>::Weak(nullptr_t) {
        
    }

    template <typename T>
    Weak<T>::Weak(Weak<T>&& other) noexcept {
        *this = std::move(other);
    }

    template <typename T>
    Weak<T>& Weak<T>::operator=(Weak<T> const& other) noexcept {
            
        if(&other == this) return *this;

        if(other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;
            this->counter->refWeak();
                
        }
            
        return *this;
            
    }

    template <typename T>
    Weak<T>& Weak<T>::operator=(Weak<T>&& other) noexcept {
            
        if(&other == this) return *this;

        if(other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;
                
            other.counter = nullptr;
            other.object = nullptr;
                
        }
            
        return *this;
            
    }

        
    template <typename T>
    String Weak<T>::getClass() const {
        return isValid() ? Type::getClassName(*object) : "nullptr";
    }

    template <typename T>
    template <typename Y>
    Weak<T>::Weak(Weak<Y> const& other) {
        *this = other;
    }
    
    template <typename T>
    bool Weak<T>::operator==(Weak<T> const& other) const noexcept {
        return other.object == object && other.counter == counter;
    }
    
    template <typename T>
    template <typename Y>
    bool Weak<T>::operator==(Weak<Y> const& other) const noexcept {
        return other.object == object && other.counter == counter;
    }

    
    template <typename T>
    template <typename Y>
    Weak<T>& Weak<T>::operator=(Weak<Y> const& other) noexcept {

        static_assert(std::is_base_of_v<T, Y>, "Y must inherit T");
            
        if (other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;
            this->counter->refWeak();
                
        }

        return *this;
            
    }

    template <typename T>
    template <typename Y>
    Weak<T>& Weak<T>::operator=(Weak<Y>&& other) noexcept {

        static_assert(std::is_base_of_v<T, Y>, "Y must inherit T");
            
        if (other.isValid()) {

            this->counter = other.counter;
            this->object = other.object;

            other.counter = nullptr;
            other.object = nullptr;
                
        }

        return *this;
            
    }

    template<typename T>
    Weak<T>::operator bool() const {
        return isValid();
    }

    template<typename T>
    template<typename Y>
    bool Weak<T>::operator==(const Y &other) const noexcept {
        if(!isValid()) return false;
        return *object == other;
    }

}


#endif