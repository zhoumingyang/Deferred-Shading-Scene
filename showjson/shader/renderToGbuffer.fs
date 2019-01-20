#version 330
uniform sampler2D tex;
uniform int isMap;
uniform vec3 lightSphereColor;
in vec2 vUv;
in vec3 vPos;
in vec3 vNormal;

void main() {
	if(isMap == 1) {
		gl_FragData[0] = texture2D(tex, vUv.xy);
	}
	else if(isMap == 0){
		gl_FragData[0] = vec4(1.0, 1.0, 1.0, 1.0);
	}
	else if(isMap == 2) {
		gl_FragData[0] = vec4(lightSphereColor, 1.0);
	}
    gl_FragData[1] = vec4(vPos.xyz, 0);
    gl_FragData[2] = vec4(vNormal.xyz, 0);
}