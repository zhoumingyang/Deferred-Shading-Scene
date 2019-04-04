#version 330
precision highp float;
precision highp int;
uniform vec3 pointColor;
uniform float opacity;
out vec4 color;

void main() {
	float step = 6.0;
	float modx = mod(gl_FragCoord.x, step);
	float mody = mod(gl_FragCoord.y, step);
	float e = 2.0;
	if((modx>=-e&&modx<=e)&&(mody>=-e&&mody<=e)) {
		color = vec4(pointColor, opacity);
	} else {
		discard;
	}
}