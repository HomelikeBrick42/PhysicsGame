#pragma once

typedef struct Renderer Renderer;

#include "Defines.h"

typedef struct Shader Shader;

Shader* Shader_Create(Renderer* renderer, const char* vertexShaderPath, const char* fragmentShaderPath);
void Shader_Destroy(Shader* shader);

#if defined(SHADER_PRIVATE)
typedef void Shader_DestroyFunc(void* shader);

typedef struct Shader {
    Shader_DestroyFunc* Destroy;
    void* Data;
} Shader;
#endif
