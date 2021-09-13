#pragma once

#include "Defines.h"
#include <math.h>

typedef union Vector2f {
    f32 Data[2];
    struct {
        f32 x, y;
    };
    struct {
        f32 r, g;
    };
    struct {
        f32 u, v;
    };
} Vector2f;

INLINE Vector2f Vector2f_Add(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

INLINE Vector2f Vector2f_Sub(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

INLINE Vector2f Vector2f_Mul(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

INLINE Vector2f Vector2f_Div(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

INLINE f32 Vector2f_Length(Vector2f v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}
