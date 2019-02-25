#version 330
in vec4 vPos;
uniform vec3 lightPos;
uniform float far_plane;
void main() {
    float lightDistance = length(vPos.xyz - lightPos) / far_plane;
    gl_FragDepth = lightDistance;
}