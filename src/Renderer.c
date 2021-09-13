#define RENDERER_PRIVATE
#include "Renderer.h"

#include "OpenGLRenderer.h"

#define SHADER_PRIVATE
#include "Shader.h"

#define VERTEX_BUFFER_PRIVATE
#include "VertexBuffer.h"

#define INDEX_BUFFER_PRIVATE
#include "IndexBuffer.h"

#include <stdlib.h>

Renderer* Renderer_Create(Window* window) {
    Renderer* renderer = malloc(sizeof(Renderer));
    if (renderer == nil) {
        return nil;
    }

    renderer->Data = OpenGLRenderer_Create(window);
    if (renderer->Data == nil) {
        free(renderer);
        return nil;
    }

    renderer->Destroy          = cast(Renderer_DestroyFunc*) OpenGLRenderer_Destroy;
    renderer->SetClearColor    = cast(Renderer_SetClearColorFunc*) OpenGLRenderer_SetClearColor;
    renderer->Clear            = cast(Renderer_ClearFunc*) OpenGLRenderer_Clear;
    renderer->BeginFrame       = cast(Renderer_BeginFrameFunc*) OpenGLRenderer_BeginFrameFunc;
    renderer->EndFrame         = cast(Renderer_EndFrameFunc*) OpenGLRenderer_EndFrameFunc;
    renderer->DrawVertexBuffer = cast(Renderer_DrawVertexBufferFunc*) OpenGLRenderer_DrawVertexBuffer;
    renderer->DrawIndexed      = cast(Renderer_DrawIndexedFunc*) OpenGLRenderer_DrawIndexed;
    renderer->OnWindowResize   = cast(Renderer_OnWindowResizeFunc*) OpenGLRenderer_OnWindowResize;
    renderer->Present          = cast(Renderer_PresentFunc*) OpenGLRenderer_Present;

    return renderer;
}

void Renderer_Destroy(Renderer* renderer) {
    renderer->Destroy(renderer->Data);
    free(renderer);
}

void Renderer_SetClearColor(Renderer* renderer, u8 red, u8 green, u8 blue) {
    renderer->SetClearColor(renderer->Data, red, green, blue);
}

void Renderer_Clear(Renderer* renderer) {
    renderer->Clear(renderer->Data);
}

void Renderer_BeginFrame(Renderer* renderer) {
    renderer->BeginFrame(renderer->Data);
}

void Renderer_EndFrame(Renderer* renderer) {
    renderer->EndFrame(renderer->Data);
}

void Renderer_DrawVertexBuffer(
    Renderer* renderer, Shader* shader, VertexBuffer* vertexBuffer, u32 count) {
    renderer->DrawVertexBuffer(renderer->Data, shader->Data, vertexBuffer->Data, count);
}

void Renderer_DrawIndexed(
    Renderer* renderer, Shader* shader, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) {
    renderer->DrawIndexed(renderer->Data, shader->Data, vertexBuffer->Data, indexBuffer->Data);
}

void Renderer_OnWindowResize(Renderer* renderer, u32 width, u32 height) {
    renderer->OnWindowResize(renderer->Data, width, height);
}

void Renderer_Present(Renderer* renderer) {
    renderer->Present(renderer->Data);
}
