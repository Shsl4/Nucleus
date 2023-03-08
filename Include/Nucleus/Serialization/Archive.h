#pragma once

#include <Nucleus/Serialization/Container.h>

namespace Nucleus {
    
    class Archive {

    public:

        Archive() = default;

        Archive(Archive const& other);

        void reset();

        NODISCARD FORCEINLINE ObjectContainer* rootContainer() const { return this->root.pointer(); }
        
    private:

        Unique<ObjectContainer> root = Unique<ObjectContainer>::make();
    
    };
    
}

#define SERIALIZATION_INLINE
#include <Nucleus/Inline/Serialization.inl>
#undef SERIALIZATION_INLINE