#include "Defines.h"
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Clock.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct GameData {
    b8 Running;
    Window* Window;
    Renderer* Renderer;
    Shader* VertexColorShader;
    VertexBuffer* TriangleVertexBuffer;
    IndexBuffer* TriangleIndexBuffer;
    f32 Delta;
    b8 WPressed;
    b8 APressed;
    b8 SPressed;
    b8 DPressed;
    b8 EPressed;
    b8 QPressed;
} GameData;

static void CloseCallback(Window* window) {
    GameData* data = Window_GetUserData(window);
    data->Running  = FALSE;
}

static void ResizeCallback(Window* window, u32 width, u32 height) {
    GameData* data = Window_GetUserData(window);
    Renderer_OnWindowResize(data->Renderer, width, height);
}

static void MousePositionCallback(Window* window, s32 xPos, s32 yPos) {
    // GameData* data = Window_GetUserData(window);
}

static void MouseMoveCallback(Window* window, s32 xDelta, s32 yDelta) {
    // GameData* data = Window_GetUserData(window);
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

        case KeyCode_E: {
            data->EPressed = pressed;
        } break;

        case KeyCode_Q: {
            data->QPressed = pressed;
        } break;

        default: {
        } break;
    }
}

static void DrawCallback(Window* window) {
    GameData* data = Window_GetUserData(window);

    Renderer_BeginFrame(data->Renderer);

    Renderer_Clear(data->Renderer);

    Renderer_DrawIndexed(
        data->Renderer, data->VertexColorShader, data->TriangleVertexBuffer, data->TriangleIndexBuffer);

    Renderer_EndFrame(data->Renderer);
    Renderer_Present(data->Renderer);
}

static const char* ColorVertexShaderSource = "#version 440 core\n"
                                             "\n"
                                             "layout(location = 0) in vec4 a_Position;\n"
                                             "layout(location = 1) in vec4 a_Color;\n"
                                             "\n"
                                             "layout(location = 0) out vec4 v_Color;\n"
                                             "\n"
                                             "layout(location = 0) uniform mat4 u_ProjectionMatrix = mat4(1.0);\n"
                                             "layout(location = 1) uniform mat4 u_ViewMatrix = mat4(1.0);\n"
                                             "layout(location = 2) uniform mat4 u_ModelMatrix = mat4(1.0);\n"
                                             "\n"
                                             "void main() {\n"
                                             "   v_Color = a_Color;"
                                             "   gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;\n"
                                             "}\n"
                                             "\n";

static const char* ColorFragmentShaderSource = "#version 440 core\n"
                                               "\n"
                                               "layout(location = 0) out vec4 a_Color;\n"
                                               "\n"
                                               "layout(location = 0) in vec4 v_Color;\n"
                                               "\n"
                                               "void main() {\n"
                                               "   a_Color = v_Color;\n"
                                               "}\n"
                                               "\n";

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
    Window_SetKeyCallback(data.Window, KeyCallback);
    Window_SetDrawCallback(data.Window, DrawCallback);

    Renderer_SetClearColor(data.Renderer, 51, 51, 51);

    data.VertexColorShader = Shader_Create(data.Renderer, ColorVertexShaderSource, ColorFragmentShaderSource);
    if (data.VertexColorShader == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create color shader!\n");
        return EXIT_FAILURE;
    }

    struct {
        struct {
            f32 x, y, z;
        } Position;
        struct {
            u8 r, g, b, a;
        } Color;
    } vertices[] = {
        { { .x = +0.0f, .y = +0.5f, .z = 0.0f }, { .r = 255, .g = 0, .b = 0, .a = 255 } },
        { { .x = +0.5f, .y = -0.5f, .z = 0.0f }, { .r = 0, .g = 0, .b = 255, .a = 255 } },
        { { .x = -0.5f, .y = -0.5f, .z = 0.0f }, { .r = 0, .g = 255, .b = 0, .a = 255 } },
    };

    VertexBufferLayoutType verticesLayoutTypes[] = {
        VertexBufferType_Float3,
        VertexBufferType_UByte4,
    };

    u8 verticesLayoutNormalized[] = {
        FALSE,
        TRUE,
    };

    _Static_assert(ArrayCount(verticesLayoutTypes) == ArrayCount(verticesLayoutNormalized), "These must be the same");

    VertexBufferLayout verticesLayout = {
        .Types        = verticesLayoutTypes,
        .Normalized   = verticesLayoutNormalized,
        .ElementCount = ArrayCount(verticesLayoutTypes),
    };

    data.TriangleVertexBuffer = VertexBuffer_Create(data.Renderer, vertices, sizeof(vertices), verticesLayout);
    if (data.TriangleVertexBuffer == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create triangle vertex buffer!\n");
        return EXIT_FAILURE;
    }

    u32 indices[] = {
        0,
        1,
        2,
    };

    data.TriangleIndexBuffer = IndexBuffer_Create(data.Renderer, indices, ArrayCount(indices));
    if (data.TriangleIndexBuffer == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create triangle index buffer!\n");
        return EXIT_FAILURE;
    }

    Window_Show(data.Window);

    Clock* clock = Clock_Create();
    if (clock == nil) {
        fflush(stdout);
        fprintf(stderr, "Failed to create clock!\n");
        return EXIT_FAILURE;
    }

    Clock_Start(clock);

    Window_DisableMouse(data.Window);

    f64 time;
    f64 lastTime = Clock_GetTime(clock);
    while (data.Running) {
        Clock_Update(clock);
        time       = Clock_GetTime(clock);
        data.Delta = cast(f32)(time - lastTime);
        lastTime   = time;

        Window_InvalidatePixels(data.Window);
        Window_PollEvents(data.Window);
    }

    Clock_Destroy(clock);

    Window_EnableMouse(data.Window);

    Window_Hide(data.Window);

    VertexBuffer_Destroy(data.TriangleVertexBuffer);
    IndexBuffer_Destroy(data.TriangleIndexBuffer);

    Shader_Destroy(data.VertexColorShader);

    Renderer_Destroy(data.Renderer);
    Window_Destroy(data.Window);
    return EXIT_SUCCESS;
}
