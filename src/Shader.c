#define SHADER_PRIVATE
#include "Shader.h"

#define RENDERER_PRIVATE
#include "Renderer.h"

#include "OpenGLShader.h"

#include <stdlib.h>

Shader* Shader_Create(Renderer* renderer, const char* vertexShaderSource, const char* fragmentShaderSource) {
    Shader* shader = malloc(sizeof(Shader));
    if (shader == nil) {
        return nil;
    }

    shader->Data = OpenGLShader_Create(renderer->Data, vertexShaderSource, fragmentShaderSource);
    if (shader->Data == nil) {
        free(shader);
        return nil;
    }

    shader->Destroy = cast(Shader_DestroyFunc*) OpenGLShader_Destroy;

    return shader;
}

void Shader_Destroy(Shader* shader) {
    shader->Destroy(shader->Data);
    free(shader);
}
