#version 330
precision highp float;
precision highp int;
const int MAX_PARALLEL_LIGHTS = 9;
const int MAX_POINT_LIGHTS = 25;

uniform sampler2D tDiffuse;
uniform sampler2D tPosition;
uniform sampler2D tNormal;
uniform int numPrleLight;
uniform int numPointLight;

struct ParallelLight {
	vec3 color;
	float ambient;
	float diffuse;
	vec3 direction;
};

struct LightAttenuation {
	float constant;
	float linear;
	float exp;
};

struct PointLight {
	vec3 color;
	float ambient;
	float diffuse;
	LightAttenuation attenuation;
	vec3 position;
};

uniform float specular;
uniform float specualrPower;
uniform vec3 eyeWorldPos;

uniform ParallelLight prleLights[MAX_PARALLEL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];

in vec2 vUv;

vec4 calcAmbientColor(ParallelLight _prleLight) {
	return vec4(_prleLight.color * _prleLight.ambient, 1.0f);
}

vec4 calcDiffuseColor(vec3 normal, ParallelLight _prleLight) {
	float diffuseFactor = dot(normal, -_prleLight.direction);
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	if(diffuseFactor > 0) {
		diffuseColor = vec4(_prleLight.color * _prleLight.diffuse * diffuseFactor, 1.0);
	}else{
		diffuseColor = vec4(0.15, 0.15, 0.15, 1.0);
	}
	return diffuseColor;
}

vec4 calcPointLight(vec3 normal, PointLight _pointLight, vec3 vPos) {
	vec3 lightDirection = vPos - _pointLight.position;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec4 ambientColor = vec4(_pointLight.color * _pointLight.ambient, 1.0f);
	float diffuseFactor = dot(normal, -lightDirection);
	vec4 diffuseColor  = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	if(diffuseFactor > 0) {
		diffuseColor = vec4(_pointLight.color * _pointLight.diffuse * diffuseFactor, 1.0f);    
		vec3 pointToEye = normalize(eyeWorldPos - vPos);
		vec3 lightReflect = normalize(reflect(lightDirection, normal));
		float specularFactor = dot(pointToEye, lightReflect);
		if(specularFactor > 0) {
			specularFactor = pow(specularFactor, specualrPower);
			specularColor = vec4(_pointLight.color * specular * specularFactor, 1.0f);
		}
	}
	vec4 tmpColor = ambientColor + diffuseColor + specularColor;
	float attenuation = _pointLight.attenuation.constant + _pointLight.attenuation.linear * distance + _pointLight.attenuation.exp * distance * distance;
	return tmpColor / attenuation;
}

void main() {
    vec4 texColor = vec4(texture2D(tDiffuse, vUv.xy).xyz, 1.0);
    vec4 position = texture2D(tPosition, vUv.xy);
    vec4 normal = texture2D(tNormal, vUv.xy);
    normal = normalize(normal);

    vec3 tmpNormal = normal.xyz;
	vec3 tmpPos = position.xyz;
    vec4 ambientColor = calcAmbientColor(prleLights[0]);
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 pointLightColor = vec4(0.0, 0.0, 0.0, 0.0);

	int tmpNumPrleLight = numPrleLight;
	int tmpNumPointLight = numPointLight;
	if(tmpNumPrleLight > MAX_PARALLEL_LIGHTS) {
		tmpNumPrleLight = MAX_PARALLEL_LIGHTS;
	}
	if(tmpNumPointLight > MAX_POINT_LIGHTS) {
		tmpNumPointLight = MAX_POINT_LIGHTS;
	}

	for(int i = 0; i < tmpNumPrleLight; i++) {
		diffuseColor += calcDiffuseColor(tmpNormal, prleLights[i]);
	}
	for(int i = 0; i < tmpNumPointLight; i++) {
		pointLightColor += calcPointLight(tmpNormal, pointLights[i], tmpPos);
	}
    gl_FragColor = texColor*(pointLightColor);
}