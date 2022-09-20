#pragma once

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <utility>

#define NODISCARD [[nodiscard]]
#define NORETURN [[noreturn]]

#ifdef _WIN32

#define FORCEINLINE __forceinline

#else

#define FORCEINLINE __attribute__((always_inline)) inline

#endif
