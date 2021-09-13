#define VERTEX_BUFFER_PRIVATE
#include "VertexBuffer.h"

#define RENDERER_PRIVATE
#include "Renderer.h"

#include "OpenGLVertexBuffer.h"

#include <stdlib.h>

VertexBuffer* VertexBuffer_Create(Renderer* renderer, void* data, u64 size, VertexBufferLayout layout) {
    VertexBuffer* vertexBuffer = malloc(sizeof(VertexBuffer));
    if (vertexBuffer == nil) {
        return nil;
    }

    vertexBuffer->Data = OpenGLVertexBuffer_Create(renderer->Data, data, size, layout);
    if (vertexBuffer->Data == nil) {
        free(vertexBuffer);
        return nil;
    }

    vertexBuffer->Destroy = cast(VertexBuffer_DestroyFunc*) OpenGLVertexBuffer_Destroy;
    vertexBuffer->SetData = cast(VertexBuffer_SetDataFunc*) OpenGLVertexBuffer_SetData;
    vertexBuffer->SetLayout = cast(VertexBuffer_SetLayoutFunc*) OpenGLVertexBuffer_SetLayout;

    return vertexBuffer;
}

void VertexBuffer_Destroy(VertexBuffer* vertexBuffer) {
    vertexBuffer->Destroy(vertexBuffer->Data);
    free(vertexBuffer);
}

void VertexBuffer_SetData(VertexBuffer* vertexBuffer, void* data, u64 size) {
    vertexBuffer->SetData(vertexBuffer->Data, data, size);
}

void VertexBuffer_SetLayout(VertexBuffer* vertexBuffer, VertexBufferLayout layout) {
    vertexBuffer->SetLayout(vertexBuffer->Data, layout);
}
