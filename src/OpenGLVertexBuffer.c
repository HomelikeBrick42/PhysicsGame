#define OPENGL_VERTEX_BUFFER_PRIVATE
#include "OpenGLVertexBuffer.h"

#include <stdlib.h>
#include <stdio.h>

static u64 GetLayoutTypeSize(VertexBufferLayoutType type) {
    switch (type) {
        case VertexBufferType_UByte:
            return sizeof(GLubyte);
        case VertexBufferType_UByte2:
            return 2 * sizeof(GLubyte);
        case VertexBufferType_UByte3:
            return 3 * sizeof(GLubyte);
        case VertexBufferType_UByte4:
            return 4 * sizeof(GLubyte);
        case VertexBufferType_Float:
            return sizeof(GLfloat);
        case VertexBufferType_Float2:
            return 2 * sizeof(GLfloat);
        case VertexBufferType_Float3:
            return 3 * sizeof(GLfloat);
        case VertexBufferType_Float4:
            return 4 * sizeof(GLfloat);
        default:
            return 0;
    }
}

static u64 GetLayoutTypeCount(VertexBufferLayoutType type) {
    switch (type) {
        case VertexBufferType_UByte:
        case VertexBufferType_Float:
            return 1;
        case VertexBufferType_UByte2:
        case VertexBufferType_Float2:
            return 2;
        case VertexBufferType_UByte3:
        case VertexBufferType_Float3:
            return 3;
        case VertexBufferType_UByte4:
        case VertexBufferType_Float4:
            return 4;
        default:
            return 0;
    }
}

static GLenum GetLayoutOpenGLType(VertexBufferLayoutType type) {
    switch (type) {
        case VertexBufferType_UByte:
        case VertexBufferType_UByte2:
        case VertexBufferType_UByte3:
        case VertexBufferType_UByte4:
            return GL_UNSIGNED_BYTE;
        case VertexBufferType_Float:
        case VertexBufferType_Float2:
        case VertexBufferType_Float3:
        case VertexBufferType_Float4:
            return GL_FLOAT;
        default:
            return 0;
    }
}

OpenGLVertexBuffer* OpenGLVertexBuffer_Create(OpenGLRenderer* renderer, void* data, u64 size, VertexBufferLayout layout) {
    OpenGLVertexBuffer* vertexBuffer = malloc(sizeof(OpenGLVertexBuffer));
    if (vertexBuffer == nil) {
        return nil;
    }

    vertexBuffer->Renderer = renderer;
    OpenGLRenderer_MakeContextCurrent(vertexBuffer->Renderer);

    vertexBuffer->Renderer->glGenVertexArrays(1, &vertexBuffer->VertexArrayID);
    vertexBuffer->Renderer->glGenBuffers(1, &vertexBuffer->VertexBufferID);
    OpenGLVertexBuffer_SetData(vertexBuffer, data, size);
    OpenGLVertexBuffer_SetLayout(vertexBuffer, layout);

    return vertexBuffer;
}

void OpenGLVertexBuffer_Destroy(OpenGLVertexBuffer* vertexBuffer) {
    OpenGLRenderer_MakeContextCurrent(vertexBuffer->Renderer);
    OpenGLVertexBuffer_Bind(vertexBuffer);

    vertexBuffer->Renderer->glDeleteVertexArrays(1, &vertexBuffer->VertexArrayID);
    vertexBuffer->Renderer->glDeleteBuffers(1, &vertexBuffer->VertexBufferID);
    free(vertexBuffer);
}

void OpenGLVertexBuffer_SetData(OpenGLVertexBuffer* vertexBuffer, void* data, u64 size) {
    OpenGLRenderer_MakeContextCurrent(vertexBuffer->Renderer);
    OpenGLVertexBuffer_Bind(vertexBuffer);

    vertexBuffer->Renderer->glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void OpenGLVertexBuffer_SetLayout(OpenGLVertexBuffer* vertexBuffer, VertexBufferLayout layout) {
    OpenGLRenderer_MakeContextCurrent(vertexBuffer->Renderer);
    OpenGLVertexBuffer_Bind(vertexBuffer);

    u64 stride = 0;
    for (u64 i = 0; i < layout.ElementCount; i++) {
        stride += GetLayoutTypeSize(layout.Types[i]);
    }

    u64 offset = 0;
    for (u64 i = 0; i < layout.ElementCount; i++) {
        vertexBuffer->Renderer->glEnableVertexAttribArray(i);
        vertexBuffer->Renderer->glVertexAttribPointer(i,
                                                      cast(GLint) GetLayoutTypeCount(layout.Types[i]),
                                                      GetLayoutOpenGLType(layout.Types[i]),
                                                      layout.Normalized[i] ? GL_TRUE : GL_FALSE,
                                                      stride,
                                                      cast(const void*) offset);
        offset += GetLayoutTypeSize(layout.Types[i]);
    }
}

void OpenGLVertexBuffer_Bind(OpenGLVertexBuffer* vertexBuffer) {
    OpenGLRenderer_MakeContextCurrent(vertexBuffer->Renderer);

    vertexBuffer->Renderer->glBindVertexArray(vertexBuffer->VertexArrayID);
    vertexBuffer->Renderer->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->VertexBufferID);
}
