#pragma once

#include <Nucleus/Serialization/Container.h>

namespace Nucleus {
    
    class Archive {

    public:

        Archive() = default;

        Archive(Archive const& other);

        void reset();

        NODISCARD FORCEINLINE ObjectContainer* root() const { return this->container.pointer(); }
        
    private:

        Unique<ObjectContainer> container = Unique<ObjectContainer>::make();
    
    };
    
}

#define SERIALIZATION_INLINE
#include <Nucleus/Inline/Serialization.inl>
#undef SERIALIZATION_INLINE