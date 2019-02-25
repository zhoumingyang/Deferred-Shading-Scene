#version 330
precision highp float;
precision highp int;
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 Uv;
layout (location = 2) in vec3 Normal;

uniform mat4 mvp;
uniform mat4 worldMatrix;
uniform mat4 normalMatrix;

out VS_OUT {
    vec3 vPos;
    vec2 vUv;
    vec3 vNormal;
}vsout;

void main() {
    vsout.vPos = (worldMatrix * vec4(Pos, 1.0)).xyz;
    vsout.vUv = Uv;
    vsout.vNormal = (normalMatrix * vec4(Normal, 1.0)).xyz;
    gl_Position = mvp * vec4(Pos, 1.0);
}