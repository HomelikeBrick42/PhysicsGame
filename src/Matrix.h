#pragma once

#include "Defines.h"
#include "Vector.h"

#include <math.h>

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

INLINE Matrix4x4f Matrix4x4f_Translation(Vector2f position) {
    Matrix4x4f result = Matrix4x4_Identity();

    result.Data[3][0] = position.x;
    result.Data[3][1] = position.y;

    return result;
}

INLINE Matrix4x4f Matrix4x4f_Rotate(f32 radians) {
    Matrix4x4f result = Matrix4x4_Identity();

    f32 c = cosf(radians);
    f32 s = sinf(radians);

    result.Data[0][0] = c;
    result.Data[0][1] = -s;
    result.Data[1][0] = s;
    result.Data[1][1] = c;

    return result;
}

INLINE Matrix4x4f Matrix4x4f_Scale(Vector2f scale) {
    Matrix4x4f result = Matrix4x4_Identity();

    result.Data[0][0] = scale.x;
    result.Data[1][1] = scale.y;

    return result;
}

INLINE Matrix4x4f Matrix4x4f_Multiply(Matrix4x4f a, Matrix4x4f b) {
    Matrix4x4f result;

    for (u64 i = 0; i < 4; i++) {
        for (u64 j = 0; j < 4; j++) {
            f32 sum = 0.0f;
            for (u64 k = 0; k < 4; k++) {
                sum += a.Data[i][k] * b.Data[k][j];
            }
            result.Data[i][j] = sum;
        }
    }

    return result;
}
