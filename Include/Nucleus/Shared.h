#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/Type.h>

namespace Nucleus {

    template<typename T>
    class Weak;
    
    class RefCounter final {

    public:
        
        RefCounter() = default;

        NODISCARD FORCEINLINE bool valid() const noexcept { return references > 0; }
        NODISCARD FORCEINLINE bool exhausted() const noexcept { return references == 0 && weakReferences == 0; }
        
        FORCEINLINE void ref() noexcept { ++references; }
        FORCEINLINE void refWeak() noexcept { ++weakReferences; }
        
        FORCEINLINE void unref() noexcept { if(references == 0) return; --references; }
        FORCEINLINE void unrefWeak() noexcept { if(weakReferences == 0) return; --weakReferences; }
        
    private:

        size_t references = 0;
        size_t weakReferences = 0;
            
    };
    
    template<typename T>
    class Shared final {

        template<typename Y>
        friend class Shared;
        
    public:
        
        template<typename... Args>
        static Shared make(Args&&... args);
        
        Shared();

        ~Shared();
        
        Shared(nullptr_t);

        template<typename Y>
        Shared(Shared<Y> const& other);

        template<typename Y>
        Shared(Shared<Y>&& other);

        Shared(Shared const& other);

        Shared(Shared&& other) noexcept;

        Shared& operator=(Shared const& other) noexcept;

        Shared& operator=(Shared&& other) noexcept;

        Shared& operator=(nullptr_t) noexcept;
        
        bool operator==(Shared const& other) const noexcept;

        template<typename Y>
        bool operator==(Shared<Y> const& other) const noexcept;

        template<typename Y>
        bool operator==(Y const& other) const noexcept;

        NODISCARD operator bool() const;

        template<typename Y>
        Shared& operator=(Shared<Y> const& other) noexcept;

        template<typename Y>
        Shared& operator=(Shared<Y>&& other) noexcept;
        
        FORCEINLINE T& operator*() { assert(object); return *object; }
        
        FORCEINLINE T* operator->() { assert(object); return object; }
        
        FORCEINLINE T& operator*() const { assert(object); return *object; }
        
        FORCEINLINE T* operator->() const { assert(object); return object; }
        
        FORCEINLINE T* pointer() { assert(object); return object; }
                
        FORCEINLINE T* pointer() const { assert(object); return object; }
        
        NODISCARD FORCEINLINE bool valid() const noexcept { return counter != nullptr && counter->valid(); }

        Weak<T> weak() const;

    private:
        
        void drop();

        T* object = nullptr;
        RefCounter* counter = nullptr;
        
    };

    template<typename T>
    class Weak final {
        
        template<typename Y>
        friend class Weak;
        
        friend Shared<T>;

    public:

        Weak() = default;
        
        ~Weak();
        
        template<typename Y>
        Weak(Weak<Y> const& other);

        Weak(nullptr_t);
        
        Weak(Weak const& other);

        Weak(Weak&& other) noexcept;
        
        Weak& operator=(Weak const& other) noexcept;

        Weak& operator=(Weak&& other) noexcept;
        
        template<typename Y>
        Weak& operator=(Weak<Y> const& other) noexcept;

        template<typename Y>
        Weak& operator=(Weak<Y>&& other) noexcept;

        template<typename Y>
        bool operator==(Y const& other) const noexcept;

        bool operator==(Weak const& other) const noexcept;

        template<typename Y>
        bool operator==(Weak<Y> const& other) const noexcept;

        NODISCARD operator bool() const;
        
        FORCEINLINE T& operator*() { assert(object); return *object; }
        
        FORCEINLINE T* operator->() { assert(object); return object; }
        
        FORCEINLINE T& operator*() const { assert(object); return *object; }
        
        FORCEINLINE T* operator->() const { assert(object); return object; }
        
        FORCEINLINE T* pointer() { assert(object); return object; }
                
        FORCEINLINE T* pointer() const { assert(object); return object; }

        NODISCARD FORCEINLINE bool valid() const noexcept { return counter != nullptr && counter->valid(); }

    private:
        
        T* object = nullptr;
        RefCounter* counter = nullptr;
        
    };
    
}

#define SHARED_INLINE
#include <Nucleus/Inline/Shared.inl>
#undef SHARED_INLINE
