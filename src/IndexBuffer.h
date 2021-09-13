#pragma once

typedef struct Renderer Renderer;

#include "Defines.h"

typedef struct IndexBuffer IndexBuffer;

IndexBuffer* IndexBuffer_Create(Renderer* renderer, u32* data, u64 count);
void IndexBuffer_Destroy(IndexBuffer* indexBuffer);

void IndexBuffer_SetData(IndexBuffer* indexBuffer, u32* data, u64 count);

u64 IndexBuffer_GetCount(IndexBuffer* indexBuffer);

#if defined(INDEX_BUFFER_PRIVATE)
typedef void IndexBuffer_DestroyFunc(void* indexBuffer);

typedef void IndexBuffer_SetDataFunc(void* indexBuffer, u32* data, u64 count);

typedef u64 IndexBuffer_GetCountFunc(void* indexBuffer);

typedef struct IndexBuffer {
    IndexBuffer_DestroyFunc* Destroy;
    IndexBuffer_SetDataFunc* SetData;
    IndexBuffer_GetCountFunc* GetCount;
    void* Data;
} IndexBuffer;
#endif
