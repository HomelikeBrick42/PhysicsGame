#pragma once

#include "IndexBuffer.h"

#define OPENGL_RENDERER_PRIVATE
#include "OpenGLRenderer.h"

typedef struct OpenGLIndexBuffer OpenGLIndexBuffer;

OpenGLIndexBuffer* OpenGLIndexBuffer_Create(OpenGLRenderer* renderer, u32* data, u64 count);
void OpenGLIndexBuffer_Destroy(OpenGLIndexBuffer* indexBuffer);

void OpenGLIndexBuffer_SetData(OpenGLIndexBuffer* indexBuffer, u32* data, u64 count);

u64 OpenGLIndexBuffer_GetCount(OpenGLIndexBuffer* indexBuffer);

void OpenGLIndexBuffer_Bind(OpenGLIndexBuffer* indexBuffer);

#if defined(OPENGL_INDEX_BUFFER_PRIVATE)
typedef struct OpenGLIndexBuffer {
    OpenGLRenderer* Renderer;
    GLuint IndexBufferID;
    u64 Count;
} OpenGLIndexBuffer;
#endif
