#version 330
precision highp float;
precision highp int;
layout (location = 0) in vec3 Pos;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(Pos, 1.0);
}