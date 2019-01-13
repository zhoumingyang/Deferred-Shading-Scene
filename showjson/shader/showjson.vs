#version 330
precision highp float;
precision highp int;
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 Uv;
layout (location = 2) in vec3 Normal;

uniform mat4 mvp;
uniform mat4 worldMatrix;
uniform mat4 normalMatrix;

out vec2 vUv;
out vec3 vPos;
out vec3 vNormal;

void main() {
    gl_Position = mvp * vec4(Pos, 1.0);
    vUv = Uv;
	vPos = (worldMatrix * vec4(Pos, 1.0)).xyz;
	vNormal = (normalMatrix * vec4(Normal, 0.0)).xyz;
}