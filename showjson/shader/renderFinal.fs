#version 330
precision highp float;
precision highp int;
uniform sampler2D sceneImage;
uniform sampler2D blurImage;
uniform bool bloom;
uniform float exposure;

in vec2 vUv;

void main() {
    const float gamma = 2.2;
    vec3 oridinaryColor = texture(sceneImage, vUv).rgb;
    vec3 bloomColor = texture(blurImage, vUv).rgb;
    if(bloom)
        oridinaryColor += 0.75 * bloomColor;
    vec3 result = vec3(1.0) - exp(-oridinaryColor * exposure);
    result = pow(result, vec3(1.0 / gamma));
    gl_FragColor = vec4(result, 1.0);
}