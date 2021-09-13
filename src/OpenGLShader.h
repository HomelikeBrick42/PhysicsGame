#pragma once

#include "Shader.h"

#define OPENGL_RENDERER_PRIVATE
#include "OpenGLRenderer.h"

typedef struct OpenGLShader OpenGLShader;

OpenGLShader* OpenGLShader_Create(OpenGLRenderer* renderer, const char* vertexShaderSource, const char* fragmentShaderSource);
void OpenGLShader_Destroy(OpenGLShader* shader);

void OpenGLShader_Bind(OpenGLShader* shader);

#if defined(OPENGL_SHADER_PRIVATE)
typedef struct OpenGLShader {
    OpenGLRenderer* Renderer;
    GLuint ShaderProgramID;
} OpenGLShader;
#endif
