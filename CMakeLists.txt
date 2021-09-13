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

add_executable(Game src/Main.c src/Window.h src/Defines.h src/Window.c src/Renderer.h src/Renderer.c src/OpenGLRenderer.h src/OpenGLRenderer.c src/VertexBuffer.h src/VertexBuffer.c src/OpenGLVertexBuffer.h src/OpenGLVertexBuffer.c src/Shader.h src/Shader.c src/OpenGLShader.h src/OpenGLShader.c src/Clock.h src/Clock.c src/IndexBuffer.h src/IndexBuffer.c src/OpenGLIndexBuffer.h src/OpenGLIndexBuffer.c src/Matrix.h src/Vector.h)
