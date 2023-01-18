#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/Allocator.h>
#include <Nucleus/Exceptions.h>
#include <Nucleus/Type.h>

namespace Nucleus {
    
    template<typename T>
    class Unique final {
        
    public:
        
        Unique() = default;

        ~Unique();

        Unique(nullptr_t);

        template<typename... Args>
        static Unique make(Args&&... args);

        Unique(Unique&& other) noexcept;

        Unique(Unique const& other) = delete;

        Unique& operator=(Unique&& other) noexcept;

        Unique& operator=(Unique const& other) = delete;

        Unique& operator=(nullptr_t);

        bool operator==(Unique const& other) const;

        bool operator==(T const& other) const;

        operator bool() const;

        T& operator*();
        
        T const& operator*() const;
        
        T* operator->();
        
        T* operator->() const;
        
        FORCEINLINE T* pointer() { assertValid(); return object; }
                
        FORCEINLINE T* pointer() const { assertValid(); return object; }

        FORCEINLINE void assertValid() const { if(!isValid()) throw Exceptions::NullPointer(); }
        
        FORCEINLINE bool isValid() const { return object != nullptr; }
        
    private:
        
        void drop();
        
        T* object = nullptr;
        
    };

}

#define UNIQUE_INLINE
#include <Nucleus/Inline/Unique.inl>
#undef UNIQUE_INLINE
