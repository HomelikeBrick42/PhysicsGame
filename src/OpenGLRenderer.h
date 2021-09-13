#pragma once

typedef struct Window Window;
typedef struct OpenGLShader OpenGLShader;
typedef struct OpenGLVertexBuffer OpenGLVertexBuffer;
typedef struct OpenGLIndexBuffer OpenGLIndexBuffer;

#include "Defines.h"

typedef struct OpenGLRenderer OpenGLRenderer;

OpenGLRenderer* OpenGLRenderer_Create(Window* window);
void OpenGLRenderer_Destroy(OpenGLRenderer* renderer);

void OpenGLRenderer_SetClearColor(OpenGLRenderer* renderer, u8 red, u8 green, u8 blue);
void OpenGLRenderer_Clear(OpenGLRenderer* renderer);

void OpenGLRenderer_BeginFrameFunc(OpenGLRenderer* renderer);
void OpenGLRenderer_EndFrameFunc(OpenGLRenderer* renderer);
void OpenGLRenderer_DrawVertexBuffer(
    OpenGLRenderer* renderer, OpenGLShader* shader, OpenGLVertexBuffer* vertexBuffer, u32 count);
void OpenGLRenderer_DrawIndexed(OpenGLRenderer* renderer,
                                OpenGLShader* shader,
                                OpenGLVertexBuffer* vertexBuffer,
                                OpenGLIndexBuffer* indexBuffer);

void OpenGLRenderer_OnWindowResize(OpenGLRenderer* renderer, u32 width, u32 height);

void OpenGLRenderer_Present(OpenGLRenderer* renderer);

void OpenGLRenderer_MakeContextCurrent(OpenGLRenderer* renderer);

typedef char GLchar;
typedef b8 GLboolean;
typedef s8 GLbyte;
typedef u8 GLubyte;
typedef s16 GLshort;
typedef u16 GLushort;
typedef s32 GLint;
typedef u32 GLuint;
typedef s32 GLfixed;
typedef s64 GLint64;
typedef u64 GLuint64;
typedef u32 GLsizei;
typedef u32 GLenum;
typedef s64 GLintptr;
typedef u64 GLsizeiptr;
typedef u64 GLsync;
typedef u32 GLbitfield;
typedef f32 GLfloat;
typedef f32 GLclampf;
typedef f64 GLdouble;
typedef f64 GLclampd;

#define GL_TRUE  1
#define GL_FALSE 0

#define GL_COLOR_BUFFER_BIT   16384
#define GL_STENCIL_BUFFER_BIT 1024
#define GL_DEPTH_BUFFER_BIT   256

#define GL_DEPTH_TEST 2929
#define GL_BLEND      3042
#define GL_CULL_FACE  2884

#define GL_SRC_ALPHA           770
#define GL_ONE_MINUS_SRC_ALPHA 771

#define GL_FRONT          1028
#define GL_BACK           1029
#define GL_FRONT_AND_BACK 1032

#define GL_ARRAY_BUFFER         34962
#define GL_ELEMENT_ARRAY_BUFFER 34963

#define GL_STATIC_DRAW 35044

#define GL_TRIANGLES 4

#define GL_FRAGMENT_SHADER 35632
#define GL_VERTEX_SHADER   35633

#define GL_COMPILE_STATUS  35713
#define GL_LINK_STATUS     35714
#define GL_INFO_LOG_LENGTH 35716

#define GL_UNSIGNED_BYTE 5121
#define GL_FLOAT         5126
#define GL_UNSIGNED_INT  5125

#define OPENGL_FUNCTIONS                                                                                            \
    OPENGL_FUNCTION(GLenum, glGetError, void)                                                                       \
    OPENGL_FUNCTION(void, glEnable, GLenum)                                                                         \
    OPENGL_FUNCTION(void, glDisable, GLenum)                                                                        \
    OPENGL_FUNCTION(void, glBlendFunc, GLenum sfactor, GLenum dfactor)                                              \
    OPENGL_FUNCTION(void, glCullFace, GLenum mode)                                                                  \
    OPENGL_FUNCTION(void, glDepthRange, GLclampd nearVal, GLclampd farVal)                                          \
                                                                                                                    \
    OPENGL_FUNCTION(void, glClearColor, f32 red, f32 green, f32 blue, f32 alpha)                                    \
    OPENGL_FUNCTION(void, glClear, GLbitfield mask)                                                                 \
                                                                                                                    \
    OPENGL_FUNCTION(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height)                              \
                                                                                                                    \
    OPENGL_FUNCTION(void, glGenVertexArrays, GLsizei n, GLuint* arrays)                                             \
    OPENGL_FUNCTION(void, glBindVertexArray, GLuint array)                                                          \
    OPENGL_FUNCTION(void,                                                                                           \
                    glVertexAttribPointer,                                                                          \
                    GLuint index,                                                                                   \
                    GLint size,                                                                                     \
                    GLenum type,                                                                                    \
                    GLboolean normalized,                                                                           \
                    GLsizei stride,                                                                                 \
                    const void* pointer)                                                                            \
    OPENGL_FUNCTION(void, glEnableVertexAttribArray, GLuint index)                                                  \
    OPENGL_FUNCTION(void, glDeleteVertexArrays, GLsizei n, const GLuint* arrays)                                    \
                                                                                                                    \
    OPENGL_FUNCTION(void, glGenBuffers, GLsizei n, GLuint* buffers)                                                 \
    OPENGL_FUNCTION(void, glBindBuffer, GLenum target, GLuint buffer)                                               \
    OPENGL_FUNCTION(void, glBufferData, GLenum target, GLsizeiptr size, const void* data, GLenum usage)             \
    OPENGL_FUNCTION(void, glDeleteBuffers, GLsizei n, const GLuint* buffers)                                        \
                                                                                                                    \
    OPENGL_FUNCTION(void, glDrawArrays, GLenum mode, GLint first, GLsizei count)                                    \
    OPENGL_FUNCTION(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const void* indices)             \
                                                                                                                    \
    OPENGL_FUNCTION(GLuint, glCreateShader, GLenum shaderType)                                                      \
    OPENGL_FUNCTION(void, glShaderSource, GLuint shader, GLsizei count, const GLchar** string, const GLint* length) \
    OPENGL_FUNCTION(void, glCompileShader, GLuint shader)                                                           \
    OPENGL_FUNCTION(void, glGetShaderiv, GLuint shader, GLenum pname, GLint* params)                                \
    OPENGL_FUNCTION(void, glGetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog)   \
    OPENGL_FUNCTION(void, glDeleteShader, GLuint shader)                                                            \
                                                                                                                    \
    OPENGL_FUNCTION(GLuint, glCreateProgram, void)                                                                  \
    OPENGL_FUNCTION(void, glAttachShader, GLuint program, GLuint shader)                                            \
    OPENGL_FUNCTION(void, glLinkProgram, GLuint program)                                                            \
    OPENGL_FUNCTION(void, glGetProgramiv, GLuint program, GLenum pname, GLint* params)                              \
    OPENGL_FUNCTION(void, glGetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog) \
    OPENGL_FUNCTION(void, glDetachShader, GLuint program, GLuint shader)                                            \
    OPENGL_FUNCTION(void, glUseProgram, GLuint program)                                                             \
    OPENGL_FUNCTION(void, glDeleteProgram, GLuint program)                                                          \
                                                                                                                    \
    OPENGL_FUNCTION(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)

#if defined(OPENGL_RENDERER_PRIVATE)
    #if defined(_WIN32) || defined(_WIN64)
        #include <windows.h>

        #define WGL_FUNCTIONS                              \
            WGL_FUNCTION(HGLRC, wglCreateContext, HDC)     \
            WGL_FUNCTION(BOOL, wglDeleteContext, HGLRC)    \
            WGL_FUNCTION(BOOL, wglMakeCurrent, HDC, HGLRC) \
            WGL_FUNCTION(PROC, wglGetProcAddress, LPCSTR)  \
            WGL_FUNCTION(HGLRC, wglGetCurrentContext, void)
    #endif

typedef struct OpenGLRenderer {
    Window* Window;
    #define OPENGL_FUNCTION(ret, name, ...) ret(_cdecl* name)(__VA_ARGS__);
    OPENGL_FUNCTIONS
    #undef OPENGL_FUNCTION
    #if defined(_WIN32) || defined(_WIN64)
    HMODULE OpenGLDLL;
        #define WGL_FUNCTION(ret, name, ...) ret (*name)(__VA_ARGS__);
    WGL_FUNCTIONS
        #undef WGL_FUNCTION
    HGLRC OpenGLContext;
    #endif
} OpenGLRenderer;
#endif
