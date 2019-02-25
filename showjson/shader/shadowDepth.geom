#version 330 core
precision highp float;
precision highp int;
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];
out vec4 vPos;

void main() {
    for (int j = 0; j < 6; j++) {
        gl_Layer = j;
        for (int i = 0; i < 3; i++) {
            vPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[j] * vPos;
            EmitVertex();
        }
    }
    EndPrimitive();
}