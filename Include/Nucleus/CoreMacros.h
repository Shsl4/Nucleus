#pragma once

#define NOMINMAX

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <utility>

#define NODISCARD [[nodiscard]]
#define UNUSED [[maybe_unused]]
#define NORETURN [[noreturn]]
#define DEPRECATED [[deprecated]]

#ifdef _WIN64

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

    /*!
     * Throws an exception of type @see{Exceptions::Exception}. Allows the library to throw
     * without having to include the Exceptions header which might cause circular dependency
     * @param message The error message
     */
    NORETURN void throwException(const char* message);

}

#define f32(x) static_cast<Nucleus::Float32>(x)
#define f64(x) static_cast<Nucleus::Float64>(x)
#define i32(x) static_cast<Nucleus::Int32>(x)
#define i64(x) static_cast<Nucleus::Int64>(x)
#define st(x) static_cast<size_t>(x)
#define nthrow(m) Nucleus::throwException(m)
#define nthrowif(b, m) if(b) Nucleus::throwException(m)
