#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

typedef u8 b8;

#define cast(type) (type)
#define nil (cast(void*) 0)

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define INLINE static inline

enum {
    FALSE = 0,
    TRUE = 1,
};
