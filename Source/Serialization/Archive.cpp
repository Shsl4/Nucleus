#include <Nucleus/Serialization/Archive.h>

namespace Nucleus{
    
    Archive::Archive(Archive const& other) {
            
        if (other.container) {
            container = Unique(other.root()->duplicate());
        }
            
    }

    void Archive::reset() {
        this->container = Unique<ObjectContainer>::make();
    }
    
}
