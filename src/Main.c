#include "Defines.h"
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Clock.h"
#include "Array.h"

#include "Matrix.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct Point {
    Vector2f Position;
    Vector2f LastPosition;
    f32 Radius;
    f32 Mass;
    b8 Fixed;
} Point;

typedef struct Stick {
    Point* A;
    Point* B;
    f32 Length;
    f32 Springiness;
    f32 Width;
} Stick;

typedef struct GameData {
    b8 Running;
    Window* Window;
    Renderer* Renderer;
    Shader* ColorShader;
    Shader* CircleShader;
    VertexBuffer* CircleVertexBuffer;
    IndexBuffer* CircleIndexBuffer;
    VertexBuffer* SquareVertexBuffer;
    IndexBuffer* SquareIndexBuffer;
    Matrix4x4f ProjectionMatrix;
    Vector2f CameraPosition;
    f32 CameraScale;
    f32 Delta;
    b8 WPressed, APressed, SPressed, DPressed;
    Array(Point*) Points; // These are pointers because stick keeps a pointer to the points
    Array(Stick*) Sticks;
} GameData;

static void CloseCallback(Window* window) {
    GameData* data = Window_GetUserData(window);
    data->Running  = FALSE;
}

static void ResizeCallback(Window* window, u32 width, u32 height) {
    GameData* data = Window_GetUserData(window);
    Renderer_OnWindowResize(data->Renderer, width, height);
    f32 aspect             = cast(f32) width / cast(f32) height;
    data->ProjectionMatrix = Matrix4x4f_Orthographic(-aspect / data->CameraScale,
                                                     aspect / data->CameraScale,
                                                     1.0f / data->CameraScale,
                                                     -1.0f / data->CameraScale,
                                                     -1.0f,
                                                     1.0f);
}

static void MousePositionCallback(Window* window, s32 xPos, s32 yPos) {
    // GameData* data = Window_GetUserData(window);
}

static void MouseMoveCallback(Window* window, s32 xDelta, s32 yDelta) {
    // GameData* data = Window_GetUserData(window);
}

static void MouseScrollCallback(Window* window, s32 xDelta, s32 yDelta) {
    GameData* data = Window_GetUserData(window);
    data->CameraScale += cast(f32) yDelta * 0.1f * data->CameraScale;
    if (data->CameraScale < 0.01f) {
        data->CameraScale = 0.01f;
    }

    u32 width, height;
    Window_GetSize(window, &width, &height);
    f32 aspect             = cast(f32) width / cast(f32) height;
    data->ProjectionMatrix = Matrix4x4f_Orthographic(-aspect / data->CameraScale,
                                                     aspect / data->CameraScale,
                                                     1.0f / data->CameraScale,
                                                     -1.0f / data->CameraScale,
                                                     -1.0f,
                                                     1.0f);
}

static void KeyCallback(Window* window, KeyCode key, b8 pressed) {
    GameData* data = Window_GetUserData(window);
    switch (key) {
        case KeyCode_Escape: {
            if (pressed) {
                if (Window_IsMouseDisabled(window)) {
                    Window_EnableMouse(window);
                } else {
                    Window_DisableMouse(window);
                }
            }
        } break;

        case KeyCode_W: {
            data->WPressed = pressed;
        } break;

        case KeyCode_S: {
            data->SPressed = pressed;
        } break;

        case KeyCode_A: {
            data->APressed = pressed;
        } break;

        case KeyCode_D: {
            data->DPressed = pressed;
        } break;

        default: {
        } break;
    }
}

static void DrawCallback(Window* window) {
    GameData* data = Window_GetUserData(window);

    Renderer_BeginFrame(data->Renderer, Matrix4x4f_Translation(data->CameraPosition), data->ProjectionMatrix);

    Renderer_Clear(data->Renderer);

    for (u64 i = 0; i < Array_GetLength(data->Sticks); i++) {
        Stick* stick = data->Sticks[i];

        Vector2f direction = Vector2f_Sub(stick->B->Position, stick->A->Position);
        f32 rotation       = atan2f(direction.x, direction.y);
        f32 distance       = Vector2f_Length(direction);
        Matrix4x4f matrix =
            Matrix4x4f_Multiply(Matrix4x4f_Scale((Vector2f){ .x = stick->Width, .y = distance }),
                                Matrix4x4f_Multiply(Matrix4x4f_Rotate(rotation), Matrix4x4f_Translation(stick->A->Position)));

        Renderer_DrawIndexed(data->Renderer, data->ColorShader, data->SquareVertexBuffer, data->SquareIndexBuffer, matrix);
    }

    for (u64 i = 0; i < Array_GetLength(data->Points); i++) {
        Point* point = data->Points[i];

        Matrix4x4f matrix = Matrix4x4f_Multiply(Matrix4x4f_Scale((Vector2f){ .x = point->Radius, .y = point->Radius }),
                                                Matrix4x4f_Translation(point->Position));

        Renderer_DrawIndexed(data->Renderer, data->CircleShader, data->CircleVertexBuffer, data->CircleIndexBuffer, matrix);
    }

    Renderer_EndFrame(data->Renderer);
    Renderer_Present(data->Renderer);
}

static void Update(GameData* data, f32 dt) {
    // Camera
    {
        if (data->WPressed) {
            data->CameraPosition.y += dt / data->CameraScale * 2.0f;
        }

        if (data->SPressed) {
            data->CameraPosition.y -= dt / data->CameraScale * 2.0f;
        }

        if (data->APressed) {
            data->CameraPosition.x -= dt / data->CameraScale * 3.0f;
        }

        if (data->DPressed) {
            data->CameraPosition.x += dt / data->CameraScale * 3.0f;
        }
    }
}

static void FixedUpdate(GameData* data, f32 dt) {
    // Points
    {
        for (u64 i = 0; i < Array_GetLength(data->Points); i++) {
            Point* pointA = data->Points[i];

            Vector2f velocity    = Vector2f_Sub(pointA->Position, pointA->LastPosition);
            pointA->LastPosition = pointA->Position;
            if (!pointA->Fixed) {
                pointA->Position = Vector2f_Add(pointA->Position, velocity);
                pointA->Position.y -= 0.01f; // Gravity
                continue;
            }
        }
    }

    const u64 SimulationCount = 2;
    for (u64 simulationIndex = 0; simulationIndex < SimulationCount; simulationIndex++) {

        // Sticks
        {
            for (u64 i = 0; i < Array_GetLength(data->Sticks); i++) {
                Stick* stick = data->Sticks[i];

                if (stick->A->Fixed && stick->B->Fixed) {
                    continue;
                }

                Vector2f direction = Vector2f_Sub(stick->B->Position, stick->A->Position);
                f32 distance       = Vector2f_Length(direction);
                f32 difference     = stick->Length - distance;

                if (!stick->A->Fixed && !stick->B->Fixed) {
                    f32 percent     = difference / distance * 0.5f * stick->Springiness;
                    Vector2f offset = Vector2f_Mul(direction, (Vector2f){ .x = percent, .y = percent });

                    stick->A->Position = Vector2f_Sub(stick->A->Position, offset);
                    stick->B->Position = Vector2f_Add(stick->B->Position, offset);
                } else if (stick->A->Fixed) {
                    f32 percent     = difference / distance * stick->Springiness;
                    Vector2f offset = Vector2f_Mul(direction, (Vector2f){ .x = percent, .y = percent });

                    stick->B->Position = Vector2f_Add(stick->B->Position, offset);
                } else if (stick->B->Fixed) {
                    f32 percent     = difference / distance * stick->Springiness;
                    Vector2f offset = Vector2f_Mul(direction, (Vector2f){ .x = percent, .y = percent });

                    stick->A->Position = Vector2f_Sub(stick->A->Position, offset);
                }
            }
        }

        for (u64 i = 0; i < Array_GetLength(data->Points); i++) {
            Point* point = data->Points[i];
            if (!point->Fixed) {
                for (u64 j = 0; j < Array_GetLength(data->Sticks); j++) {
                    Stick* stick = data->Sticks[j];

                    if (stick->A == point || stick->B == point) {
                        continue;
                    }

                    Vector2f squareUpDirection = Vector2f_Sub(stick->B->Position, stick->A->Position);
                    f32 squareRotation         = atan2f(squareUpDirection.x, squareUpDirection.y);
                    f32 squareHeight           = Vector2f_Length(squareUpDirection);

                    Vector2f squarePosition = stick->A->Position;

                    Vector2f pointRelPos = Vector2f_Sub(point->Position, squarePosition);

                    f32 s = sinf(squareRotation);
                    f32 c = cosf(squareRotation);

                    Vector2f rotatedPointRelPos = {
                        .x = pointRelPos.x * c - pointRelPos.y * s,
                        .y = pointRelPos.x * s + pointRelPos.y * c,
                    };

                    Vector2f rotatedClosestPoint = rotatedPointRelPos;
                    if (rotatedClosestPoint.x < -stick->Width * 0.5f) {
                        rotatedClosestPoint.x = -stick->Width * 0.5f;
                    } else if (rotatedClosestPoint.x > stick->Width * 0.5f) {
                        rotatedClosestPoint.x = stick->Width * 0.5f;
                    }

                    if (rotatedClosestPoint.y < 0.0f) {
                        rotatedClosestPoint.y = 0.0f;
                    } else if (rotatedClosestPoint.y > squareHeight) {
                        rotatedClosestPoint.y = squareHeight;
                    }

                    s = sinf(-squareRotation);
                    c = cosf(-squareRotation);

                    Vector2f closestPoint = {
                        .x = (rotatedClosestPoint.x * c - rotatedClosestPoint.y * s) + squarePosition.x,
                        .y = (rotatedClosestPoint.x * s + rotatedClosestPoint.y * c) + squarePosition.y,
                    };

                    Vector2f direction = Vector2f_Sub(closestPoint, point->Position);
                    f32 sqrDistance    = Vector2f_SqrLength(direction);
                    if (sqrDistance < point->Radius * point->Radius) {
                        f32 distance   = sqrtf(sqrDistance);
                        f32 difference = point->Radius - distance;
                        f32 percent    = difference / distance * 0.5f;
                        Vector2f offset = Vector2f_Mul(direction, (Vector2f){ .x = percent, .y = percent });
                        point->Position = Vector2f_Sub(point->Position, offset);
                    }
                }
            }

            for (u64 j = i + 1; j < Array_GetLength(data->Points); j++) {
                Point* pointA = point;
                Point* pointB = data->Points[j];

                Vector2f direction = Vector2f_Sub(pointB->Position, pointA->Position);
                f32 sqrDistance    = Vector2f_SqrLength(direction);
                if (sqrDistance < (pointA->Radius + pointB->Radius) * (pointA->Radius + pointB->Radius)) {
                    f32 distance    = sqrtf(sqrDistance);
                    f32 difference  = (pointA->Radius + pointB->Radius) - distance;
                    f32 percent     = difference / distance * 0.5f;
                    Vector2f offset = Vector2f_Mul(direction, (Vector2f){ .x = percent, .y = percent });
                    if (!pointA->Fixed && !pointB->Fixed) {
                        pointA->Position = Vector2f_Sub(pointA->Position, offset);
                        pointB->Position = Vector2f_Add(pointB->Position, offset);
                    } else if (pointA->Fixed) {
                        percent *= 2.0f;
                        pointB->Position = Vector2f_Add(pointB->Position, offset);
                    } else if (pointB->Fixed) {
                        percent *= 2.0f;
                        pointA->Position = Vector2f_Sub(pointA->Position, offset);
                    }
                }
            }
        }
    }
}

int main() {
    GameData data = {};
    data.Running  = TRUE;

    data.Points = Array_Create(Point*);
    data.Sticks = Array_Create(Stick*);

    data.CameraScale = 0.1f;

    data.Window = Window_Create(640, 480);
    if (data.Window == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create window!\n");
        return EXIT_FAILURE;
    }

    data.Renderer = Renderer_Create(data.Window);
    if (data.Renderer == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create renderer!\n");
        return EXIT_FAILURE;
    }

    Window_SetUserData(data.Window, &data);
    Window_SetCloseCallback(data.Window, CloseCallback);
    Window_SetResizeCallback(data.Window, ResizeCallback);
    Window_SetMousePositionCallback(data.Window, MousePositionCallback);
    Window_SetMouseMoveCallback(data.Window, MouseMoveCallback);
    Window_SetMouseScrollCallback(data.Window, MouseScrollCallback);
    Window_SetKeyCallback(data.Window, KeyCallback);
    Window_SetDrawCallback(data.Window, DrawCallback);

    Renderer_SetClearColor(data.Renderer, 51, 51, 51);

    data.ColorShader =
        Shader_Create(data.Renderer, "assets/shaders/ColorShader.vert.glsl", "assets/shaders/ColorShader.frag.glsl");
    if (data.ColorShader == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create color shader!\n");
        return EXIT_FAILURE;
    }

    data.CircleShader =
        Shader_Create(data.Renderer, "assets/shaders/CircleShader.vert.glsl", "assets/shaders/CircleShader.frag.glsl");
    if (data.CircleShader == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create circle shader!\n");
        return EXIT_FAILURE;
    }

    {
        typedef struct CircleShaderVertex {
            Vector2f Position;
            struct {
                u8 r, g, b, a;
            } Color;
            Vector2f Coord;
        } CircleShaderVertex;

        CircleShaderVertex vertices[] = {
            { { .x = -1.0f, .y = +1.0f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = -1.0f, .y = +1.0f } },
            { { .x = +1.0f, .y = +1.0f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = +1.0f, .y = +1.0f } },
            { { .x = +1.0f, .y = -1.0f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = +1.0f, .y = -1.0f } },
            { { .x = -1.0f, .y = -1.0f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = -1.0f, .y = -1.0f } },
        };

        u32 indices[] = {
            0, 1, 2, 0, 2, 3,
        };

        VertexBufferLayoutType layoutTypes[] = {
            VertexBufferType_Float2,
            VertexBufferType_UByte4,
            VertexBufferType_Float2,
        };

        b8 layoutNormalized[] = {
            FALSE,
            TRUE,
            FALSE,
        };

        _Static_assert(ArrayCount(layoutTypes) == ArrayCount(layoutNormalized), "These must be the same");

        VertexBufferLayout layout = {
            .Types        = layoutTypes,
            .Normalized   = layoutNormalized,
            .ElementCount = ArrayCount(layoutTypes),
        };

        data.CircleVertexBuffer = VertexBuffer_Create(data.Renderer, vertices, sizeof(vertices), layout);
        if (data.CircleVertexBuffer == nil) {
            fflush(stdout);
            fprintf(stderr, "Failed to create circle vertex buffer!\n");
            return EXIT_FAILURE;
        }

        data.CircleIndexBuffer = IndexBuffer_Create(data.Renderer, indices, ArrayCount(indices));
        if (data.CircleIndexBuffer == nil) {
            fflush(stdout);
            fprintf(stderr, "Failed to create circle index buffer!\n");
            return EXIT_FAILURE;
        }
    }

    {
        typedef struct ColorShaderVertex {
            Vector2f Position;
            struct {
                u8 r, g, b, a;
            } Color;
        } ColorShaderVertex;

        ColorShaderVertex vertices[] = {
            { { .x = -0.5f, .y = +1.0f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
            { { .x = +0.5f, .y = +1.0f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
            { { .x = +0.5f, .y = -0.0f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
            { { .x = -0.5f, .y = -0.0f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
        };

        u32 indices[] = {
            0, 1, 2, 0, 2, 3,
        };

        VertexBufferLayoutType layoutTypes[] = {
            VertexBufferType_Float2,
            VertexBufferType_UByte4,
        };

        b8 layoutNormalized[] = {
            FALSE,
            TRUE,
        };

        _Static_assert(ArrayCount(layoutTypes) == ArrayCount(layoutNormalized), "These must be the same");

        VertexBufferLayout layout = {
            .Types        = layoutTypes,
            .Normalized   = layoutNormalized,
            .ElementCount = ArrayCount(layoutTypes),
        };

        data.SquareVertexBuffer = VertexBuffer_Create(data.Renderer, vertices, sizeof(vertices), layout);
        if (data.SquareVertexBuffer == nil) {
            fflush(stdout);
            fprintf(stderr, "Failed to create square vertex buffer!\n");
            return EXIT_FAILURE;
        }

        data.SquareIndexBuffer = IndexBuffer_Create(data.Renderer, indices, ArrayCount(indices));
        if (data.SquareIndexBuffer == nil) {
            fflush(stdout);
            fprintf(stderr, "Failed to create square index buffer!\n");
            return EXIT_FAILURE;
        }
    }

    {
        Point* point        = calloc(1, sizeof(Point));
        point->Position.x   = -9.0f;
        point->LastPosition = point->Position;
        point->Radius       = 0.5f;
        point->Mass         = 1.0f;
        point->Fixed        = TRUE;
        Array_Push(data.Points, point);
    }

    {
        Point* point        = calloc(1, sizeof(Point));
        point->Position.x   = -6.0f;
        point->LastPosition = point->Position;
        point->Radius       = 0.5f;
        point->Mass         = 1.0f;
        point->Fixed        = FALSE;
        Array_Push(data.Points, point);
    }

    {
        Point* point        = calloc(1, sizeof(Point));
        point->Position.x   = -3.0f;
        point->LastPosition = point->Position;
        point->Radius       = 0.5f;
        point->Mass         = 1.0f;
        point->Fixed        = FALSE;
        Array_Push(data.Points, point);
    }

    {
        Point* point        = calloc(1, sizeof(Point));
        point->Position.x   = 3.0f;
        point->LastPosition = point->Position;
        point->Radius       = 0.5f;
        point->Mass         = 1.0f;
        point->Fixed        = FALSE;
        Array_Push(data.Points, point);
    }

    {
        Point* point        = calloc(1, sizeof(Point));
        point->Position.x   = 6.0f;
        point->LastPosition = point->Position;
        point->Radius       = 0.5f;
        point->Mass         = 1.0f;
        point->Fixed        = FALSE;
        Array_Push(data.Points, point);
    }

    {
        Point* point        = calloc(1, sizeof(Point));
        point->Position.x   = 9.0f;
        point->LastPosition = point->Position;
        point->Radius       = 0.5f;
        point->Mass         = 1.0f;
        point->Fixed        = TRUE;
        Array_Push(data.Points, point);
    }

    {
        Point* point        = calloc(1, sizeof(Point));
        point->Position.x   = 5.8f;
        point->Position.y   = 10.0f;
        point->LastPosition = point->Position;
        point->Radius       = 1.0f;
        point->Mass         = 1.0f;
        point->Fixed        = FALSE;
        Array_Push(data.Points, point);
    }

    {
        Stick* stick       = calloc(1, sizeof(Stick));
        stick->A           = data.Points[0];
        stick->B           = data.Points[1];
        stick->Length      = 3.0f;
        stick->Springiness = 0.1f;
        stick->Width       = 0.25f;
        Array_Push(data.Sticks, stick);
    }

    {
        Stick* stick       = calloc(1, sizeof(Stick));
        stick->A           = data.Points[1];
        stick->B           = data.Points[2];
        stick->Length      = 3.0f;
        stick->Springiness = 0.1f;
        stick->Width       = 0.25f;
        Array_Push(data.Sticks, stick);
    }

    {
        Stick* stick       = calloc(1, sizeof(Stick));
        stick->A           = data.Points[2];
        stick->B           = data.Points[3];
        stick->Length      = 3.0f;
        stick->Springiness = 0.1f;
        stick->Width       = 0.25f;
        Array_Push(data.Sticks, stick);
    }

    {
        Stick* stick       = calloc(1, sizeof(Stick));
        stick->A           = data.Points[3];
        stick->B           = data.Points[4];
        stick->Length      = 3.0f;
        stick->Springiness = 0.1f;
        stick->Width       = 0.25f;
        Array_Push(data.Sticks, stick);
    }

    {
        Stick* stick       = calloc(1, sizeof(Stick));
        stick->A           = data.Points[4];
        stick->B           = data.Points[5];
        stick->Length      = 3.0f;
        stick->Springiness = 0.1f;
        stick->Width       = 0.25f;
        Array_Push(data.Sticks, stick);
    }

    Window_Show(data.Window);

    Clock* clock = Clock_Create();
    if (clock == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create clock!\n");
        return EXIT_FAILURE;
    }

    Clock_Start(clock);

    f64 time;
    f64 lastTime                  = Clock_GetTime(clock);
    f32 fixedUpdateTime           = 0.0f;
    const f32 FixedUpdateInterval = 1.0f / 60.0f;
    while (data.Running) {
        Clock_Update(clock);
        time       = Clock_GetTime(clock);
        data.Delta = cast(f32)(time - lastTime);
        lastTime   = time;

        Update(&data, data.Delta);

        fixedUpdateTime += data.Delta;
        while (fixedUpdateTime - FixedUpdateInterval > 0.0f) {
            FixedUpdate(&data, FixedUpdateInterval);
            fixedUpdateTime -= FixedUpdateInterval;
        }

        Window_InvalidatePixels(data.Window);
        Window_PollEvents(data.Window);
    }

    Clock_Destroy(clock);

    if (Window_IsMouseDisabled(data.Window)) {
        Window_EnableMouse(data.Window);
    }

    Window_Hide(data.Window);

    for (u64 i = 0; i < Array_GetLength(data.Sticks); i++) {
        free(data.Sticks[i]);
    }
    Array_Destroy(data.Sticks);

    for (u64 i = 0; i < Array_GetLength(data.Points); i++) {
        free(data.Points[i]);
    }
    Array_Destroy(data.Points);

    VertexBuffer_Destroy(data.SquareVertexBuffer);
    IndexBuffer_Destroy(data.SquareIndexBuffer);

    VertexBuffer_Destroy(data.CircleVertexBuffer);
    IndexBuffer_Destroy(data.CircleIndexBuffer);

    Shader_Destroy(data.CircleShader);
    Shader_Destroy(data.ColorShader);

    Renderer_Destroy(data.Renderer);
    Window_Destroy(data.Window);
    return EXIT_SUCCESS;
}
