#version 460

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 50
#define MAX_SPOTLIGHTS 50

// -----------------------------------------------
// ----- Inputs from vertex shader ---------------
// -----------------------------------------------

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

// -----------------------------------------------
// ----- Outputs from this shader ----------------
// -----------------------------------------------

layout(location = 0) out vec4 outColour;

// -----------------------------------------------
// ----- Light structs ---------------------------
// -----------------------------------------------

struct DirectionalLight {
	vec3 direction;
	vec3 colour;
};

struct PointLight {
	vec3 position;
	vec3 colour;
	float attenuation;
};

struct Spotlight {
	vec3 position;
	vec3 direction;
	vec3 colour;
	float attenuation;
	float innerCutoffAngle;
	float outerCutoffAngle;
};

// -----------------------------------------------
// ----- Material struct -------------------------
// -----------------------------------------------

struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D emissionMap;
	sampler2D bumpMap;
	float roughness;
};

// -----------------------------------------------
// ----- Uniforms --------------------------------
// -----------------------------------------------

uniform vec3 uCameraPosition;

uniform vec3 uAmbientColour;
uniform float uAmbientPower;

uniform int uNumDirectionalLights;
uniform int uNumPointLights;
uniform int uNumSpotlights;
uniform DirectionalLight uDirectionalLights[MAX_DIR_LIGHTS];
uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform Spotlight uSpotlights[MAX_SPOTLIGHTS];

uniform Material uMaterial;

// -----------------------------------------------
// ----- Functions -------------------------------
// -----------------------------------------------

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection);
vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 viewDirection);

// -----------------------------------------------
// ----- MAIN ------------------------------------
// -----------------------------------------------

void main() {
	vec3 normal = normalize(inNormal);
	vec3 viewDirection = normalize(uCameraPosition - inWorldPosition);
	
	vec3 result = uAmbientColour * uAmbientPower * texture(uMaterial.diffuseMap, inUV).xyz;

	for(int i = 0; i < uNumDirectionalLights; ++i) {
		if(i >= MAX_DIR_LIGHTS) break;
		result += calculateDirectionalLight(uDirectionalLights[i], normal, viewDirection);
	}

	for(int i = 0; i < uNumPointLights; ++i) {
		if(i >= MAX_POINT_LIGHTS) break;
		result += calculatePointLight(uPointLights[i], normal, viewDirection);
	}

	for(int i = 0; i < uNumSpotlights; ++i) {
		if(i >= MAX_SPOTLIGHTS) break;
		result += calculateSpotlight(uSpotlights[i], normal, viewDirection);
	}

	vec3 emission = texture(uMaterial.emissionMap, inUV).rgb;
	result += emission;

	outColour = vec4(result, 1.0);
}

// -----------------------------------------------
// ----- Function defintions ---------------------
// -----------------------------------------------

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
	vec3 lightDirection = normalize(-light.direction);
	
	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * texture(uMaterial.diffuseMap, inUV).xyz;

	vec3 reflectedLightDirection = reflect(-lightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), (1.0 - clamp(uMaterial.roughness, 0.0, 1.0)) * 256);
	vec3 specular = light.colour * specularPower * texture(uMaterial.specularMap, inUV).xyz;

	return diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection) {
	vec3 lightToPositionDifference = light.position - inWorldPosition;
	vec3 lightDirection = normalize(lightToPositionDifference);

	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * texture(uMaterial.diffuseMap, inUV).xyz;

	// vec3 reflectedLightDirection = reflect(-lightDirection, normal);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularPower = pow(max(dot(viewDirection, halfwayDirection), 0.0), 256 - (256 * clamp(uMaterial.roughness, 0.0, 1.0)));
	vec3 specular = light.colour * specularPower * texture(uMaterial.specularMap, inUV).xyz;

	float dist = length(lightToPositionDifference);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(dist, 2.0));

	return attenuation * (diffuse + specular);
}

vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 viewDirection) {
	vec3 lightToPositionDifference = light.position - inWorldPosition;
	vec3 lightDirection = normalize(lightToPositionDifference);

	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * texture(uMaterial.diffuseMap, inUV).xyz;

	vec3 reflectedLightDirection = reflect(-lightDirection, normal);
	float specularPower = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), 256 - (256 * clamp(uMaterial.roughness, 0.0, 1.0)));
	vec3 specular = light.colour * specularPower * texture(uMaterial.specularMap, inUV).xyz;

	float dist = length(lightToPositionDifference);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(dist, 2.0));

	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = cos(light.innerCutoffAngle) - cos(light.outerCutoffAngle);
	float intensity = clamp((theta - cos(light.outerCutoffAngle)) / epsilon, 0.0, 1.0);

	return intensity * attenuation * (diffuse + specular);
}