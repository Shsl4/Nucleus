#include "AllocationObserver.h"
#include <iostream>

namespace TinyCore {

static AllocationObserver observer;

size_t AllocationObserver::allocSize = 0;
size_t AllocationObserver::allocCount = 0;
size_t AllocationObserver::releaseCount = 0;

AllocationObserver::~AllocationObserver(){
    info();
}

void AllocationObserver::info() {
			
	std::cout << "========================================\n\n";
	std::cout << "Allocation info: " << std::endl;
	std::cout << "Allocation count: " << allocCount << std::endl;
	std::cout << "Released count: " << releaseCount << std::endl;
	std::cout << "Allocated size: " << allocSize << " bytes\n\n";
	std::cout << "========================================\n";

}

}