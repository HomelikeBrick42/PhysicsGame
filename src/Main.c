#include "Defines.h"
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Clock.h"

#include "Matrix.h"

#include <stdlib.h>
#include <stdio.h>

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
} GameData;

static void CloseCallback(Window* window) {
    GameData* data = Window_GetUserData(window);
    data->Running  = FALSE;
}

static void ResizeCallback(Window* window, u32 width, u32 height) {
    GameData* data = Window_GetUserData(window);
    Renderer_OnWindowResize(data->Renderer, width, height);
    f32 aspect = cast(f32) width / cast(f32) height;
    data->ProjectionMatrix =
        Matrix4x4f_Orthographic(-aspect / data->CameraScale, aspect / data->CameraScale, 1.0f, -1.0f, -1.0f, 1.0f);
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

    Matrix4x4f circleMatrix = Matrix4x4f_Translation((Vector2f){ .x = -1.0f, .y = 0.0f });

    static f32 rotation = 0.0f;
    rotation += data->Delta * 1.5f; // I know that this is bad to update in draw function
    Matrix4x4f squareMatrix =
        Matrix4x4f_Multiply(Matrix4x4f_Rotate(rotation), Matrix4x4f_Translation((Vector2f){ .x = 1.0f, .y = 0.0f }));

    Renderer_DrawIndexed(data->Renderer, data->CircleShader, data->CircleVertexBuffer, data->CircleIndexBuffer, circleMatrix);
    Renderer_DrawIndexed(data->Renderer, data->ColorShader, data->SquareVertexBuffer, data->SquareIndexBuffer, squareMatrix);

    Renderer_EndFrame(data->Renderer);
    Renderer_Present(data->Renderer);
}

int main() {
    GameData data = {};
    data.Running  = TRUE;

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
            { { .x = -0.5f, .y = +0.5f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = -1.0f, .y = +1.0f } },
            { { .x = +0.5f, .y = +0.5f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = +1.0f, .y = +1.0f } },
            { { .x = +0.5f, .y = -0.5f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = +1.0f, .y = -1.0f } },
            { { .x = -0.5f, .y = -0.5f }, { .r = 150, .g = 0, .b = 0, .a = 255 }, { .x = -1.0f, .y = -1.0f } },
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
            { { .x = -0.5f, .y = +0.5f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
            { { .x = +0.5f, .y = +0.5f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
            { { .x = +0.5f, .y = -0.5f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
            { { .x = -0.5f, .y = -0.5f }, { .r = 0, .g = 150, .b = 0, .a = 255 } },
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

    data.CameraScale = 1.0f;

    Window_Show(data.Window);

    Clock* clock = Clock_Create();
    if (clock == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create clock!\n");
        return EXIT_FAILURE;
    }

    Clock_Start(clock);

    f64 time;
    f64 lastTime = Clock_GetTime(clock);
    while (data.Running) {
        Clock_Update(clock);
        time       = Clock_GetTime(clock);
        data.Delta = cast(f32)(time - lastTime);
        lastTime   = time;

        if (data.WPressed) {
            data.CameraPosition.y += data.Delta / data.CameraScale * 2.0f;
        }

        if (data.SPressed) {
            data.CameraPosition.y -= data.Delta / data.CameraScale * 2.0f;
        }

        if (data.APressed) {
            data.CameraPosition.x -= data.Delta / data.CameraScale * 3.0f;
        }

        if (data.DPressed) {
            data.CameraPosition.x += data.Delta / data.CameraScale * 3.0f;
        }

        Window_InvalidatePixels(data.Window);
        Window_PollEvents(data.Window);
    }

    Clock_Destroy(clock);

    if (Window_IsMouseDisabled(data.Window)) {
        Window_EnableMouse(data.Window);
    }

    Window_Hide(data.Window);

    Shader_Destroy(data.CircleShader);
    Shader_Destroy(data.ColorShader);

    Renderer_Destroy(data.Renderer);
    Window_Destroy(data.Window);
    return EXIT_SUCCESS;
}
