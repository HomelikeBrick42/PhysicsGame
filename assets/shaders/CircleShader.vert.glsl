#version 440 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Coord;

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_Coord;

layout(location = 0) uniform mat4 u_ProjectionMatrix = mat4(1.0);
layout(location = 1) uniform mat4 u_ViewMatrix = mat4(1.0);
layout(location = 2) uniform mat4 u_ModelMatrix = mat4(1.0);

void main() {
    v_Color = a_Color;
    v_Coord = a_Coord;
    // TODO: Replace put inverse in renderer
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;
}
