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

INLINE Vector2f Matrix4x4f_MultiplyVector2f(Matrix4x4f a, Vector2f b) {
    Vector2f result;

    for (u64 i = 0; i < 2; i++) {
        f32 sum = 0.0f;
        for (u64 k = 0; k < 2; k++) {
            sum += a.Data[i][k] * b.Data[k];
        }
        result.Data[i] = sum;
    }

    return result;
}

INLINE Matrix4x4f Matrix4x4f_Inverse(Matrix4x4f matrix) {
    f32* m = cast(f32*) matrix.Data;

    f32 t0  = m[10] * m[15];
    f32 t1  = m[14] * m[11];
    f32 t2  = m[6] * m[15];
    f32 t3  = m[14] * m[7];
    f32 t4  = m[6] * m[11];
    f32 t5  = m[10] * m[7];
    f32 t6  = m[2] * m[15];
    f32 t7  = m[14] * m[3];
    f32 t8  = m[2] * m[11];
    f32 t9  = m[10] * m[3];
    f32 t10 = m[2] * m[7];
    f32 t11 = m[6] * m[3];
    f32 t12 = m[8] * m[13];
    f32 t13 = m[12] * m[9];
    f32 t14 = m[4] * m[13];
    f32 t15 = m[12] * m[5];
    f32 t16 = m[4] * m[9];
    f32 t17 = m[8] * m[5];
    f32 t18 = m[0] * m[13];
    f32 t19 = m[12] * m[1];
    f32 t20 = m[0] * m[9];
    f32 t21 = m[8] * m[1];
    f32 t22 = m[0] * m[5];
    f32 t23 = m[4] * m[1];

    Matrix4x4f result;
    f32* o = cast(f32*) result.Data;

    o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
    o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
    o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
    o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

    f32 d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

    o[0]  = d * o[0];
    o[1]  = d * o[1];
    o[2]  = d * o[2];
    o[3]  = d * o[3];
    o[4]  = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
    o[5]  = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
    o[6]  = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
    o[7]  = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
    o[8]  = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
    o[9]  = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
    o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
    o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
    o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
    o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
    o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
    o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

    return result;
}
