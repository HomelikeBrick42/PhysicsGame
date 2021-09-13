#version 440 core

layout(location = 0) out vec4 a_Color;

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_Coord;

void main() {
    if (length(v_Coord) > 1.0) {
        discard;
    }
    a_Color = v_Color;
}
