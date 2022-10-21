#pragma once

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <utility>

#define NODISCARD [[nodiscard]]
#define UNUSED [[maybe_unused]]
#define NORETURN [[noreturn]]
#define DEPRECATED [[deprecated]]

#ifdef _WIN32

#define FORCEINLINE __forceinline

#else

#define FORCEINLINE __attribute__((always_inline)) inline

#endif

namespace Nucleus {
    
    using Float32 = float;
    using Float64 = double;
    
    using Int = int32_t;
    using Int8 = char;
    using Int16 = int16_t;
    using Int32 = int32_t;
    using Int64 = int64_t;

    using UInt = uint32_t;
    using UInt8 = uint8_t;
    using UInt16 = uint16_t;
    using UInt32 = uint32_t;
    using UInt64 = uint64_t;
    
}
