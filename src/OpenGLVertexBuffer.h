#pragma once

#include "VertexBuffer.h"

#define OPENGL_RENDERER_PRIVATE
#include "OpenGLRenderer.h"

typedef struct OpenGLVertexBuffer OpenGLVertexBuffer;

OpenGLVertexBuffer* OpenGLVertexBuffer_Create(OpenGLRenderer* renderer, void* data, u64 size, VertexBufferLayout layout);
void OpenGLVertexBuffer_Destroy(OpenGLVertexBuffer* vertexBuffer);

void OpenGLVertexBuffer_SetData(OpenGLVertexBuffer* vertexBuffer, void* data, u64 size);
void OpenGLVertexBuffer_SetLayout(OpenGLVertexBuffer* vertexBuffer, VertexBufferLayout layout);

void OpenGLVertexBuffer_Bind(OpenGLVertexBuffer* vertexBuffer);

#if defined(OPENGL_VERTEX_BUFFER_PRIVATE)
typedef struct OpenGLVertexBuffer {
    OpenGLRenderer* Renderer;
    GLuint VertexArrayID;
    GLuint VertexBufferID;
} OpenGLVertexBuffer;
#endif
