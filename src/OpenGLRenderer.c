#define OPENGL_RENDERER_PRIVATE
#include "OpenGLRenderer.h"

#define WINDOW_PRIVATE
#include "Window.h"

#include "OpenGLShader.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"

#include <stdlib.h>
#include <stdio.h>

static void* GetGLFunc(OpenGLRenderer* renderer, const char* name) {
    void* func = renderer->wglGetProcAddress(name);
    if (func == cast(void*) 0 || func == cast(void*) 1 || func == cast(void*) 2 || func == cast(void*) 3 ||
        func == cast(void*) - 1) {
        func = GetProcAddress(renderer->OpenGLDLL, name);
    }
    return func;
}

OpenGLRenderer* OpenGLRenderer_Create(Window* window) {
    OpenGLRenderer* renderer = malloc(sizeof(OpenGLRenderer));
    if (renderer == nil) {
        return nil;
    }

    renderer->Window = window;

#if defined(_WIN32) || defined(_WIN64)
    renderer->OpenGLDLL = LoadLibraryA("OpenGL32.dll");
    if (renderer->OpenGLDLL == nil) {
        goto ErrorNoLibrary;
    }

    #define WGL_FUNCTION(ret, name, ...)                                                                   \
        renderer->name = cast(ret(*)(__VA_ARGS__)) cast(void*) GetProcAddress(renderer->OpenGLDLL, #name); \
        if (renderer->name == nil) {                                                                       \
            goto ErrorNoContext;                                                                           \
        }
    WGL_FUNCTIONS
    #undef WGL_FUNCTION

    PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
        .nSize        = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion     = 1,
        .dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType   = PFD_TYPE_RGBA,
        .cColorBits   = 32,
        .cDepthBits   = 24,
        .cStencilBits = 8,
        .iLayerType   = PFD_MAIN_PLANE,
    };

    int format = ChoosePixelFormat(renderer->Window->DeviceContext, &pixelFormatDescriptor);
    if (format == 0) {
        goto ErrorNoContext;
    }

    if (!SetPixelFormat(renderer->Window->DeviceContext, format, &pixelFormatDescriptor)) {
        goto ErrorNoContext;
    }

    renderer->OpenGLContext = renderer->wglCreateContext(renderer->Window->DeviceContext);
    if (renderer->OpenGLContext == nil) {
        goto ErrorNoContext;
    }

    OpenGLRenderer_MakeContextCurrent(renderer);

    #define OPENGL_FUNCTION(ret, name, ...)                                    \
        renderer->name = cast(ret(*)(__VA_ARGS__)) GetGLFunc(renderer, #name); \
        if (renderer->name == nil) {                                           \
            goto Error;                                                        \
        }
    OPENGL_FUNCTIONS
    #undef OPENGL_FUNCTION
#endif

    // renderer->glEnable(GL_DEPTH_TEST);
    // renderer->glDepthRange(0.0f, 1.0f);

    renderer->glEnable(GL_BLEND);
    renderer->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TODO: glEnable(GL_CULL_FACE)

    return renderer;
Error:
    renderer->wglDeleteContext(renderer->OpenGLContext);
ErrorNoContext:
    FreeLibrary(renderer->OpenGLDLL);
ErrorNoLibrary:
    free(renderer);
    return nil;
}

void OpenGLRenderer_Destroy(OpenGLRenderer* renderer) {
#if defined(_WIN32) || defined(_WIN64)
    renderer->wglDeleteContext(renderer->OpenGLContext);
    FreeLibrary(renderer->OpenGLDLL);
#endif
    free(renderer);
}

void OpenGLRenderer_SetClearColor(OpenGLRenderer* renderer, u8 red, u8 green, u8 blue) {
    OpenGLRenderer_MakeContextCurrent(renderer);
    renderer->glClearColor(
        cast(f32) red * (1.0f / 255.0f), cast(f32) green * (1.0f / 255.0f), cast(f32) blue * (1.0f / 255.0f), 1.0f);
}

void OpenGLRenderer_Clear(OpenGLRenderer* renderer) {
    OpenGLRenderer_MakeContextCurrent(renderer);
    renderer->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer_BeginFrameFunc(OpenGLRenderer* renderer, Matrix4x4f viewMatrix, Matrix4x4f projectionMatrix) {
    renderer->ViewMatrix = Matrix4x4f_Inverse(viewMatrix);
    renderer->ProjectionMatrix = projectionMatrix;
}

void OpenGLRenderer_EndFrameFunc(OpenGLRenderer* renderer) {
    renderer->ViewMatrix = Matrix4x4_Identity();
    renderer->ProjectionMatrix = Matrix4x4_Identity();
}

void OpenGLRenderer_DrawVertexBuffer(
    OpenGLRenderer* renderer, OpenGLShader* shader, OpenGLVertexBuffer* vertexBuffer, u32 count, Matrix4x4f modelMatrix) {
    OpenGLRenderer_MakeContextCurrent(renderer);
    OpenGLShader_Bind(shader);
    // TODO: Detect if the same shader is being bound and not setting the uniforms
    renderer->glUniformMatrix4fv(0, 1, GL_FALSE, cast(GLfloat*) renderer->ProjectionMatrix.Data);
    renderer->glUniformMatrix4fv(1, 1, GL_FALSE, cast(GLfloat*) renderer->ViewMatrix.Data);
    renderer->glUniformMatrix4fv(2, 1, GL_FALSE, cast(GLfloat*) modelMatrix.Data);
    OpenGLVertexBuffer_Bind(vertexBuffer);
    renderer->glDrawArrays(GL_TRIANGLES, 0, count);
}

void OpenGLRenderer_DrawIndexed(OpenGLRenderer* renderer,
                                OpenGLShader* shader,
                                OpenGLVertexBuffer* vertexBuffer,
                                OpenGLIndexBuffer* indexBuffer,
                                Matrix4x4f modelMatrix) {
    OpenGLRenderer_MakeContextCurrent(renderer);
    OpenGLShader_Bind(shader);
    // TODO: Detect if the same shader is being bound and not setting the uniforms
    renderer->glUniformMatrix4fv(0, 1, GL_FALSE, cast(GLfloat*) renderer->ProjectionMatrix.Data);
    renderer->glUniformMatrix4fv(1, 1, GL_FALSE, cast(GLfloat*) renderer->ViewMatrix.Data);
    renderer->glUniformMatrix4fv(2, 1, GL_FALSE, cast(GLfloat*) modelMatrix.Data);
    OpenGLVertexBuffer_Bind(vertexBuffer);
    OpenGLIndexBuffer_Bind(indexBuffer);
    renderer->glDrawElements(GL_TRIANGLES, OpenGLIndexBuffer_GetCount(indexBuffer), GL_UNSIGNED_INT, nil);
}

void OpenGLRenderer_OnWindowResize(OpenGLRenderer* renderer, u32 width, u32 height) {
    OpenGLRenderer_MakeContextCurrent(renderer);
    renderer->glViewport(0, 0, width, height);
}

void OpenGLRenderer_Present(OpenGLRenderer* renderer) {
#if defined(_WIN32) || defined(_WIN64)
    SwapBuffers(renderer->Window->DeviceContext);
#endif
}

void OpenGLRenderer_MakeContextCurrent(OpenGLRenderer* renderer) {
#if defined(_WIN32) || defined(_WIN64)
    if (renderer->wglGetCurrentContext() != renderer->OpenGLContext) {
        renderer->wglMakeCurrent(renderer->Window->DeviceContext, renderer->OpenGLContext);
    }
#endif
}
