#define INDEX_BUFFER_PRIVATE
#include "IndexBuffer.h"

#define RENDERER_PRIVATE
#include "Renderer.h"

#include "OpenGLIndexBuffer.h"

#include <stdlib.h>

IndexBuffer* IndexBuffer_Create(Renderer* renderer, u32* data, u64 count) {
    IndexBuffer* indexBuffer = malloc(sizeof(IndexBuffer));
    if (indexBuffer == nil) {
        return nil;
    }

    indexBuffer->Data = OpenGLIndexBuffer_Create(renderer->Data, data, count);
    if (indexBuffer->Data == nil) {
        return nil;
    }

    indexBuffer->Destroy = cast(IndexBuffer_DestroyFunc*) OpenGLIndexBuffer_Destroy;
    indexBuffer->SetData = cast(IndexBuffer_SetDataFunc*) OpenGLIndexBuffer_SetData;
    indexBuffer->GetCount = cast(IndexBuffer_GetCountFunc*) OpenGLIndexBuffer_GetCount;

    return indexBuffer;
}

void IndexBuffer_Destroy(IndexBuffer* indexBuffer) {
    indexBuffer->Destroy(indexBuffer->Data);
    free(indexBuffer);
}

void IndexBuffer_SetData(IndexBuffer* indexBuffer, u32* data, u64 count) {
    indexBuffer->SetData(indexBuffer->Data, data, count);
}

u64 IndexBuffer_GetCount(IndexBuffer* indexBuffer) {
    return indexBuffer->GetCount(indexBuffer->Data);
}
