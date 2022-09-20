#pragma once

#include "CoreMacros.h"

namespace TinyCore {

class AllocationObserver {

	public:

		~AllocationObserver();

	private:

		template<typename T>
		friend class Allocator;

		template<typename T>
		static void statAlloc(size_t size){

			allocSize += sizeof(T) * size;
			++allocCount;

		}

		template<typename T>
		static void statRelease() {
			++releaseCount;
		}

		static void info();

		static size_t allocSize;
		static size_t allocCount;
		static size_t releaseCount;

	};

}