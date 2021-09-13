#pragma once

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
