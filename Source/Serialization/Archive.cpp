#include <Nucleus/Serialization/Archive.h>

namespace Nucleus{
    
    Archive::Archive(Archive const& other) {
            
        if (other.root) {
            root = Unique(other.rootContainer()->duplicate());
        }
            
    }

    void Archive::reset() {
        this->root = Unique<ObjectContainer>::make();
    }
    
}
