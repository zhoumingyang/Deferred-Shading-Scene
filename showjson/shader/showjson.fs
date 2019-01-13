#version 330
precision highp float;
precision highp int;
const int MAX_PARALLEL_LIGHTS = 9;
const int MAX_POINT_LIGHTS = 25;
const int MAX_SPOT_LIGHTS = 9;

in vec2 vUv;
in vec3 vPos;
in vec3 vNormal;

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

struct SpotLight {
	vec3 color;
	float ambient;
	float diffuse;
	vec3 position;
	vec3 direction;
	float cutoff;
	LightAttenuation attenuation;
};

uniform float specular;
uniform float specualrPower;
uniform vec3 eyeWorldPos;
uniform sampler2D tex;
uniform int texMap;

uniform int numPrleLight;
uniform int numPointLight;
uniform int numSpotLight;
uniform ParallelLight prleLights[MAX_PARALLEL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

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

vec4 calcPointLight(vec3 normal, PointLight _pointLight) {
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

vec4 calcSpotLight(vec3 normal, SpotLight _spotLight) {
	vec3 lightToPoint = normalize(vPos - _spotLight.position);
	float spotFactor = dot(lightToPoint, _spotLight.direction);
	if(spotFactor > _spotLight.cutoff) {
		vec3 lightDirection = vPos - _spotLight.position;
		float distance = length(lightDirection);
		lightDirection = normalize(lightDirection);

		vec4 ambientColor = vec4(_spotLight.color * _spotLight.ambient, 1.0f);
		float diffuseFactor = dot(normal, -lightDirection);
		vec4 diffuseColor  = vec4(0.0, 0.0, 0.0, 0.0);
		vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
		if(diffuseFactor > 0) {
			diffuseColor = vec4(_spotLight.color * _spotLight.diffuse * diffuseFactor, 1.0f);    
			vec3 pointToEye = normalize(eyeWorldPos - vPos);
			vec3 lightReflect = normalize(reflect(lightDirection, normal));
			float specularFactor = dot(pointToEye, lightReflect);
			if(specularFactor > 0) {
				specularFactor = pow(specularFactor, specualrPower);
				specularColor = vec4(_spotLight.color * specular * specularFactor, 1.0f);
			}
		}
		vec4 tmpColor = ambientColor + diffuseColor + specularColor;
		float attenuation = _spotLight.attenuation.constant + _spotLight.attenuation.linear * distance + _spotLight.attenuation.exp * distance * distance;
		vec4 color = tmpColor / attenuation;
		return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - _spotLight.cutoff));
	}else{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
}

void main() {
	vec3 normal = normalize(vNormal);
	vec4 ambientColor = calcAmbientColor(prleLights[0]);

	int numPrle = numPrleLight;
	int numPoint = numPointLight;
	int numSpot = numSpotLight;
	
	if(numPrle > MAX_PARALLEL_LIGHTS) {
		numPrle = MAX_PARALLEL_LIGHTS;
	}
	if(numPoint > MAX_POINT_LIGHTS) {
		numPoint = MAX_POINT_LIGHTS;
	}
	if(numSpot > MAX_SPOT_LIGHTS) {
		numSpot = MAX_SPOT_LIGHTS;
	}

	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < numPrle; i++) {
		diffuseColor += calcDiffuseColor(normal, prleLights[i]);
	}

	vec4 spotLightColor = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < numSpot; i++) {
		spotLightColor += calcSpotLight(normal, spotLights[i]);
	}

	vec4 pointLightColor = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < numPoint; i++) {
		pointLightColor += calcPointLight(normal, pointLights[i]);
	}

	if(texMap == 1) {
		gl_FragColor = texture2D(tex, vUv.xy)*(pointLightColor);
	}else{
		gl_FragColor = pointLightColor;
	}
}