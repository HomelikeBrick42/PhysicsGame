#define OPENGL_SHADER_PRIVATE
#include "OpenGLShader.h"

#include <stdlib.h>

static GLuint CreateShader(OpenGLShader* shader, const char* source, GLenum type) {
    GLuint shaderID = shader->Renderer->glCreateShader(type);
    shader->Renderer->glShaderSource(shaderID, 1, &source, nil);
    shader->Renderer->glCompileShader(shaderID);

    // TODO: Error handling

    return shaderID;
}

OpenGLShader* OpenGLShader_Create(OpenGLRenderer* renderer, const char* vertexShaderSource, const char* fragmentShaderSource) {
    OpenGLShader* shader = malloc(sizeof(OpenGLShader));
    if (shader == nil) {
        return nil;
    }

    shader->Renderer = renderer;
    OpenGLRenderer_MakeContextCurrent(shader->Renderer);

    GLuint vertexShaderID = CreateShader(shader, vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = CreateShader(shader, fragmentShaderSource, GL_FRAGMENT_SHADER);

    shader->ShaderProgramID = shader->Renderer->glCreateProgram();

    shader->Renderer->glAttachShader(shader->ShaderProgramID, vertexShaderID);
    shader->Renderer->glAttachShader(shader->ShaderProgramID, fragmentShaderID);

    shader->Renderer->glLinkProgram(shader->ShaderProgramID);

    // TODO: Error handling

    shader->Renderer->glDetachShader(shader->ShaderProgramID, vertexShaderID);
    shader->Renderer->glDeleteShader(vertexShaderID);
    shader->Renderer->glDetachShader(shader->ShaderProgramID, fragmentShaderID);
    shader->Renderer->glDeleteShader(fragmentShaderID);

    return shader;
}

void OpenGLShader_Destroy(OpenGLShader* shader) {
    OpenGLRenderer_MakeContextCurrent(shader->Renderer);
    shader->Renderer->glDeleteProgram(shader->ShaderProgramID);
}

void OpenGLShader_Bind(OpenGLShader* shader) {
    OpenGLRenderer_MakeContextCurrent(shader->Renderer);
    shader->Renderer->glUseProgram(shader->ShaderProgramID);
}
