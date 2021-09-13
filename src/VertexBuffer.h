#pragma once

typedef struct Renderer Renderer;

#include "Defines.h"

typedef struct VertexBuffer VertexBuffer;

typedef enum VertexBufferLayoutType {
    VertexBufferType_UByte,
    VertexBufferType_UByte2,
    VertexBufferType_UByte3,
    VertexBufferType_UByte4,
    VertexBufferType_Float,
    VertexBufferType_Float2,
    VertexBufferType_Float3,
    VertexBufferType_Float4,
} VertexBufferLayoutType;

typedef struct VertexBufferLayout {
    VertexBufferLayoutType* Types;
    b8* Normalized;
    u64 ElementCount;
} VertexBufferLayout;

VertexBuffer* VertexBuffer_Create(Renderer* renderer, void* data, u64 size, VertexBufferLayout layout);
void VertexBuffer_Destroy(VertexBuffer* vertexBuffer);

void VertexBuffer_SetData(VertexBuffer* vertexBuffer, void* data, u64 size);
void VertexBuffer_SetLayout(VertexBuffer* vertexBuffer, VertexBufferLayout layout);

#if defined(VERTEX_BUFFER_PRIVATE)
typedef void VertexBuffer_DestroyFunc(void* vertexBuffer);

typedef void VertexBuffer_SetDataFunc(void* vertexBuffer, void* data, u64 size);
typedef void VertexBuffer_SetLayoutFunc(void* vertexBuffer, VertexBufferLayout layout);

typedef struct VertexBuffer {
    VertexBuffer_DestroyFunc* Destroy;
    VertexBuffer_SetDataFunc* SetData;
    VertexBuffer_SetLayoutFunc* SetLayout;
    void* Data;
} VertexBuffer;
#endif
