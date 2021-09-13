#define OPENGL_SHADER_PRIVATE
#include "OpenGLShader.h"

#include <stdlib.h>
#include <stdio.h>

static GLuint CreateShader(OpenGLShader* shader, const char* path, GLenum type) {
    FILE* file = fopen(path, "rb");
    if (file == nil) {
        return 0;
    }

    fseek(file, 0, SEEK_END);
    u64 fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    GLchar* source = malloc(fileSize + 1);
    if (source == nil) {
        fclose(file);
        return 0;
    }

    if (fread(source, sizeof(GLchar), fileSize, file) != fileSize) {
        free(source);
        fclose(file);
        return 0;
    }

    source[fileSize] = '\0';

    GLuint shaderID = shader->Renderer->glCreateShader(type);
    shader->Renderer->glShaderSource(shaderID, 1, cast(const GLchar**) &source, nil);
    shader->Renderer->glCompileShader(shaderID);

    // TODO: Error handling

    return shaderID;
}

OpenGLShader* OpenGLShader_Create(OpenGLRenderer* renderer, const char* vertexShaderPath, const char* fragmentShaderPath) {
    OpenGLShader* shader = malloc(sizeof(OpenGLShader));
    if (shader == nil) {
        return nil;
    }

    shader->Renderer = renderer;
    OpenGLRenderer_MakeContextCurrent(shader->Renderer);

    GLuint vertexShaderID   = CreateShader(shader, vertexShaderPath, GL_VERTEX_SHADER);
    if (vertexShaderID == 0) {
        free(shader);
        return nil;
    }

    GLuint fragmentShaderID = CreateShader(shader, fragmentShaderPath, GL_FRAGMENT_SHADER);
    if (fragmentShaderID == 0) {
        free(shader);
        return nil;
    }

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
