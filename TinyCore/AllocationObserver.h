#pragma once

#include "CoreMacros.h"
#include <typeinfo>
#include <iostream>

namespace TinyCore {

class AllocationObserver {

	public:

		~AllocationObserver();

	private:

		template<typename T>
		friend class Allocator;

		template<typename T>
		static void statAlloc(size_t size);

		template<typename T>
		static void statRelease() {
			++releaseCount;
		}

		static void info() noexcept;

		static size_t allocSize;
		static size_t allocCount;
		static size_t releaseCount;

	};

}

#define OBSERVER_INLINE
#include "Inline/AllocationObserver.inl"
#undef OBSERVER_INLINE