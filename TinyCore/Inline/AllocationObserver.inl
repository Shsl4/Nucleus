#pragma once

#ifdef OBSERVER_INLINE

#include "../Type.h"

namespace TinyCore{
    
    template<typename T>
    void AllocationObserver::statAlloc(size_t size) {

        ++allocCount;
        allocSize += sizeof(T) * size;
        
    }

    static const AllocationObserver observer;

    size_t AllocationObserver::allocSize = 0;
    size_t AllocationObserver::allocCount = 0;
    size_t AllocationObserver::releaseCount = 0;

    inline AllocationObserver::~AllocationObserver(){
        info();
    }

    inline void AllocationObserver::info() noexcept {
			
        std::cout << "========================================\n\n";
        std::cout << "Allocation info: " << std::endl;
        std::cout << "Allocation count: " << allocCount << std::endl;
        std::cout << "Released count: " << releaseCount << std::endl;
        std::cout << "Allocated size: " << allocSize << " bytes\n\n";
        std::cout << "========================================\n";

    }

}

#endif
