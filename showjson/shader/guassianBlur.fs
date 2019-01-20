#version 330
precision highp float;
precision highp int;
out vec4 FragColor;
in vec2 vUv;
uniform sampler2D brightImage;
uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
void main() {
    vec2 tex_offset = 1.0 / textureSize(brightImage, 0);
    vec3 result = texture(brightImage, vUv).rgb * weight[0];
    if(horizontal) {
        for(int i = 1; i < 5; i++) {
            result += texture(brightImage, vUv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(brightImage, vUv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for(int i = 1; i < 5; i++) {
             result += texture(brightImage, vUv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(brightImage, vUv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
	//vec3 result = texture(brightImage, vUv).rgb * weight[0];
    FragColor = vec4(result, 1.0);
}