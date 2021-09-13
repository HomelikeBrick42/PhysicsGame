#define OPENGL_INDEX_BUFFER_PRIVATE
#include "OpenGLIndexBuffer.h"

#include <stdlib.h>

OpenGLIndexBuffer* OpenGLIndexBuffer_Create(OpenGLRenderer* renderer, u32* data, u64 count) {
    OpenGLIndexBuffer* indexBuffer = malloc(sizeof(OpenGLIndexBuffer));
    if (indexBuffer == nil) {
        return nil;
    }

    indexBuffer->Renderer = renderer;
    OpenGLRenderer_MakeContextCurrent(indexBuffer->Renderer);

    indexBuffer->Renderer->glGenBuffers(1, &indexBuffer->IndexBufferID);
    OpenGLIndexBuffer_SetData(indexBuffer, data, count);

    return indexBuffer;
}

void OpenGLIndexBuffer_Destroy(OpenGLIndexBuffer* indexBuffer) {
    OpenGLRenderer_MakeContextCurrent(indexBuffer->Renderer);
    indexBuffer->Renderer->glDeleteBuffers(1, &indexBuffer->IndexBufferID);
    free(indexBuffer);
}

void OpenGLIndexBuffer_SetData(OpenGLIndexBuffer* indexBuffer, u32* data, u64 count) {
    OpenGLRenderer_MakeContextCurrent(indexBuffer->Renderer);
    OpenGLIndexBuffer_Bind(indexBuffer);
    indexBuffer->Renderer->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data, GL_STATIC_DRAW);
    indexBuffer->Count = count;
}

u64 OpenGLIndexBuffer_GetCount(OpenGLIndexBuffer* indexBuffer) {
    return indexBuffer->Count;
}

void OpenGLIndexBuffer_Bind(OpenGLIndexBuffer* indexBuffer) {
    OpenGLRenderer_MakeContextCurrent(indexBuffer->Renderer);
    indexBuffer->Renderer->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->IndexBufferID);
}
