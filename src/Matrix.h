#pragma once

#include "Defines.h"

typedef struct Matrix4x4f {
    f32 Data[4][4];
} Matrix4x4f;

INLINE Matrix4x4f Matrix4x4_Identity() {
    Matrix4x4f result = {};
    result.Data[0][0] = 1.0f;
    result.Data[1][1] = 1.0f;
    result.Data[2][2] = 1.0f;
    result.Data[3][3] = 1.0f;
    return result;
}

INLINE Matrix4x4f Matrix4x4f_Orthographic(f32 left, f32 right, f32 top, f32 bottom, f32 nearClip, f32 farClip) {
    Matrix4x4f result;

    result.Data[0][0] = 2.0f / (right - left);
    result.Data[1][1] = 2.0f / (top - bottom);
    result.Data[2][2] = 2.0f / (farClip - nearClip);
    result.Data[3][3] = 1.0f;

    result.Data[3][0] = -(right + left) / (right - left);
    result.Data[3][1] = -(top + bottom) / (top - bottom);
    result.Data[3][2] = -(farClip + nearClip) / (farClip - nearClip);

    return result;
}
