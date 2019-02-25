#version 330
precision highp float;
precision highp int;
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Uv;
layout (location = 2) in vec3 Normal;
uniform mat4 worldMatrix;

void main() {
    gl_Position = worldMatrix * vec4(Pos, 1.0);
}