cmake_minimum_required(VERSION 3.20)
project(Game C)

set(CMAKE_C_STANDARD 11)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC")
    add_compile_options(-W4) # Wall on MSVC has hundreds of warnings in the std libary, windows header, etc
else ()
    add_compile_options(-Wall -Wextra)
endif ()
add_compile_options(-Werror -Wno-unused-parameter)

add_compile_options(-D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS)

include_directories(src)

add_executable(
        Game
        src/Clock.c
        src/Clock.h
        src/Defines.h
        src/IndexBuffer.c
        src/IndexBuffer.h
        src/Main.c
        src/Matrix.h
        src/OpenGLIndexBuffer.c
        src/OpenGLIndexBuffer.h
        src/OpenGLRenderer.c
        src/OpenGLRenderer.h
        src/OpenGLShader.c
        src/OpenGLShader.h
        src/OpenGLVertexBuffer.c
        src/OpenGLVertexBuffer.h
        src/Renderer.c
        src/Renderer.h
        src/Shader.c
        src/Shader.h
        src/Vector.h
        src/VertexBuffer.c
        src/VertexBuffer.h
        src/Window.c
        src/Window.h)
