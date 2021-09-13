#version 440 core

layout(location = 0) out vec4 a_Color;

layout(location = 0) in vec4 v_Color;

void main() {
   a_Color = v_Color;
}
