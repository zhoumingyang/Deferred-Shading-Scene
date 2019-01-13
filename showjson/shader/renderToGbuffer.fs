#version 330
uniform sampler2D tex;
uniform int isMap;
in vec2 vUv;
in vec3 vPos;
in vec3 vNormal;

void main() {
	if(isMap == 1) {
		gl_FragData[0] = texture2D(tex, vUv.xy);
	}
	else{
		gl_FragData[0] = vec4(1.0, 1.0, 1.0, 1.0);
	}
    gl_FragData[1] = vec4(vPos.xyz, 0);
    gl_FragData[2] = vec4(vNormal.xyz, 0);
}