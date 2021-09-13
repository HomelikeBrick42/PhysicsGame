#pragma once

typedef struct Window Window;
typedef struct Shader Shader;
typedef struct VertexBuffer VertexBuffer;
typedef struct IndexBuffer IndexBuffer;

#include "Defines.h"

typedef struct Renderer Renderer;

Renderer* Renderer_Create(Window* window);
void Renderer_Destroy(Renderer* renderer);

void Renderer_SetClearColor(Renderer* renderer, u8 red, u8 green, u8 blue);
void Renderer_Clear(Renderer* renderer);

void Renderer_BeginFrame(Renderer* renderer);
void Renderer_EndFrame(Renderer* renderer);
void Renderer_DrawVertexBuffer(Renderer* renderer, Shader* shader, VertexBuffer* vertexBuffer, u32 count);
void Renderer_DrawIndexed(Renderer* renderer, Shader* shader, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);

void Renderer_OnWindowResize(Renderer* renderer, u32 width, u32 height);

void Renderer_Present(Renderer* renderer);

#if defined(RENDERER_PRIVATE)
typedef void Renderer_DestroyFunc(void* renderer);

typedef void Renderer_SetClearColorFunc(void* renderer, u8 red, u8 green, u8 blue);
typedef void Renderer_ClearFunc(void* renderer);

typedef void Renderer_BeginFrameFunc(void* renderer);
typedef void Renderer_EndFrameFunc(void* renderer);
typedef void Renderer_DrawVertexBufferFunc(void* renderer, void* shader, void* vertexBuffer, u32 count);
typedef void Renderer_DrawIndexedFunc(void* renderer, void* shader, void* vertexBuffer, void* indexBuffer);

typedef void Renderer_OnWindowResizeFunc(void* renderer, u32 width, u32 height);

typedef void Renderer_PresentFunc(void* renderer);

typedef struct Renderer {
    Renderer_DestroyFunc* Destroy;
    Renderer_SetClearColorFunc* SetClearColor;
    Renderer_ClearFunc* Clear;
    Renderer_BeginFrameFunc* BeginFrame;
    Renderer_EndFrameFunc* EndFrame;
    Renderer_DrawVertexBufferFunc* DrawVertexBuffer;
    Renderer_DrawIndexedFunc* DrawIndexed;
    Renderer_OnWindowResizeFunc* OnWindowResize;
    Renderer_PresentFunc* Present;
    void* Data;
} Renderer;
#endif
