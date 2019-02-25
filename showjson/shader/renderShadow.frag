#version 330
precision highp float;
precision highp int;
const int MAX_POINT_LIGHTS = 100;

in VS_OUT {
    vec3 vPos;
    vec2 vUv;
    vec3 vNormal;
}fsin;

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
uniform sampler2D tex;
uniform samplerCube depthMap;
uniform int texMap;
uniform float farPlane;
uniform int shadowMap;

uniform int numPointLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

float calcShadow(vec3 worldPos, vec3 normal, PointLight _pointLight) {
    vec3 posToLight = worldPos - _pointLight.position;
    float closestDepth = texture(depthMap, posToLight).r;
    closestDepth *= farPlane;
    float currentDepth = length(posToLight);
    //float bias = 0.05; //some hard code now
	float bias = max(20 * (1.0 - dot(normal, normalize(posToLight))), 20);
	float shadow = currentDepth - bias > closestDepth ? 0.25 : 0.0;
    return shadow;
}

vec4 calcPointLight(vec3 normal, PointLight _pointLight) {
	vec3 lightDirection = fsin.vPos - _pointLight.position;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec4 ambientColor = vec4(_pointLight.color * _pointLight.ambient, 1.0f);
	float diffuseFactor = dot(normal, -lightDirection);
	vec4 diffuseColor  = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	if(diffuseFactor > 0) {
		diffuseColor = vec4(_pointLight.color * _pointLight.diffuse * diffuseFactor, 1.0f);    
		vec3 pointToEye = normalize(eyeWorldPos - fsin.vPos);
		vec3 lightReflect = normalize(reflect(lightDirection, normal));
		float specularFactor = dot(pointToEye, lightReflect);
		if(specularFactor > 0) {
			specularFactor = pow(specularFactor, specualrPower);
			specularColor = vec4(_pointLight.color * specular * specularFactor, 1.0f);
		}
	}
	float shadow = 0.0;
	if(shadowMap == 1) {
		shadow = calcShadow(fsin.vPos, normal, _pointLight);
	}
	vec4 tmpColor = ambientColor + (1- shadow) * (diffuseColor + specularColor);
	float attenuation = _pointLight.attenuation.constant + _pointLight.attenuation.linear * distance + _pointLight.attenuation.exp * distance * distance;
	return tmpColor / attenuation;
}

void main() {
    vec3 normal = normalize(fsin.vNormal);
    int tmpNumPointLight = numPointLight;
    if (tmpNumPointLight > MAX_POINT_LIGHTS) {
        tmpNumPointLight = MAX_POINT_LIGHTS;
    }
    vec4 pointLightColor = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < tmpNumPointLight; i++) {
		pointLightColor += calcPointLight(normal, pointLights[i]);
	}

    if(texMap == 1) {
		gl_FragColor = texture2D(tex, fsin.vUv.xy) * (pointLightColor);
	} else {
		gl_FragColor = pointLightColor;
	}

	//vec3 tmVertexToLight = fsin.vPos - pointLights[0].position;
	//float tmClosestDepth = texture(depthMap, tmVertexToLight).r;
	//gl_FragColor = vec4(vec3(tmClosestDepth), 1.0);
}