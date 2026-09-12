#pragma once

#ifdef __cplusplus

#include <cstdint>
#include <cstddef>

#else

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#endif

typedef uint8_t u8;   ///<  8-bit unsigned integer
typedef uint16_t u16; ///< 16-bit unsigned integer
typedef uint32_t u32; ///< 32-bit unsigned integer
typedef uint64_t u64; ///< 64-bit unsigned integer

typedef int8_t s8;   ///<  8-bit signed integer
typedef int16_t s16; ///< 16-bit signed integer
typedef int32_t s32; ///< 32-bit signed integer
typedef int64_t s64; ///< 64-bit signed integer

typedef volatile u8 vu8;   ///<  8-bit volatile unsigned integer.
typedef volatile u16 vu16; ///< 16-bit volatile unsigned integer.
typedef volatile u32 vu32; ///< 32-bit volatile unsigned integer.
typedef volatile u64 vu64; ///< 64-bit volatile unsigned integer.

typedef volatile s8 vs8;   ///<  8-bit volatile signed integer.
typedef volatile s16 vs16; ///< 16-bit volatile signed integer.
typedef volatile s32 vs32; ///< 32-bit volatile signed integer.
typedef volatile s64 vs64; ///< 64-bit volatile signed integer.

typedef float f32;
typedef double f64;

typedef s32 Result;


/// Creates a bitmask from a bit number.
#define BIT(n) (1U<<(n))

/// Aligns a struct (and other types?) to m, making sure that the size of the struct is a multiple of m.
#define ALIGN(m)   __attribute__((aligned(m)))
/// Packs a struct (and other types?) so it won't include padding bytes.
#define PACKED     __attribute__((packed))

#define USED       __attribute__((used))
#define UNUSED     __attribute__((unused))

/// Flags a function as deprecated.
#define DEPRECATED __attribute__ ((deprecated))

#define NAKED __attribute__((naked))
#define NORETURN __attribute__((noreturn))

#define FORCE_INLINE __attribute__((always_inline))
#define NOINLINE __attribute__((noinline))

#define LIKELY(x) __builtin_expect((x), true)
#define UNLIKELY(x) __builtin_expect((x), false)

#define THUMB  __attribute__((target("thumb")))
#define ARM __attribute__((target("arm")))

#define ASSERT_SIZE(type, size) static_assert(sizeof(type) == (size))
#define ASSERT_OFFSET(type, member, offset) static_assert(offsetof(type, member) == (offset))

#define DO_CONCAT(x, y) x##y
#define CONCAT(x, y) DO_CONCAT(x, y)

#define INSERT_PAD(num_bytes) u8 CONCAT(pad, __LINE__)[(num_bytes)]

#ifdef __cplusplus

typedef char8_t char8;
typedef char16_t char16;

#endif
