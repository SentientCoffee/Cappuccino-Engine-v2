#version 460

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 50
#define MAX_SPOTLIGHTS 50

// -----------------------------------------------
// ----- Input/output values ---------------------
// -----------------------------------------------

layout(location = 0) in DATA {
	vec2 uv;
} inFrag;

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
// ----- Structs ---------------------------------
// -----------------------------------------------

// Pseudo material (for lighting)
struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 normal;
	vec3 emission;
	vec3 bump;
	float roughness;
} mat;

// GBuffer texture inputs
struct GBuffer {
	sampler2D position;
	sampler2D normal;
	sampler2D albedo;
	sampler2D specRough;
	sampler2D emission;
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

uniform GBuffer uGBuffer;

// -----------------------------------------------
// ----- Functions -------------------------------
// -----------------------------------------------

vec3 calculateDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection);
vec3 calculateSpotlight(Spotlight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection);

// -----------------------------------------------
// ----- MAIN ------------------------------------
// -----------------------------------------------

void main() {
	vec3 worldPosition = texture(uGBuffer.position, inFrag.uv).rgb;
	vec3 normal = texture(uGBuffer.normal, inFrag.uv).rgb;
	
	mat.diffuse   = texture(uGBuffer.albedo,    inFrag.uv).rgb;
	mat.emission  = texture(uGBuffer.emission,  inFrag.uv).rgb;
	mat.specular  = texture(uGBuffer.specRough, inFrag.uv).rrr;
	mat.roughness = texture(uGBuffer.specRough, inFrag.uv).g;

	vec3 viewDirection = normalize(uCameraPosition - worldPosition);
	vec3 result = uAmbientColour * uAmbientPower * mat.diffuse;

	for(int i = 0; i < uNumDirectionalLights; ++i) {
		if(i >= MAX_DIR_LIGHTS) break;
		result += calculateDirectionalLight(uDirectionalLights[i], mat, normal, viewDirection);
	}

	for(int i = 0; i < uNumPointLights; ++i) {
		if(i >= MAX_POINT_LIGHTS) break;
		result += calculatePointLight(uPointLights[i], mat, worldPosition, normal, viewDirection);
	}

	for(int i = 0; i < uNumSpotlights; ++i) {
		if(i >= MAX_SPOTLIGHTS) break;
		result += calculateSpotlight(uSpotlights[i], mat, worldPosition, normal, viewDirection);
	}

	result += mat.emission;
	outColour = vec4(result, 1.0);
}

// -----------------------------------------------
// ----- Function defintions ---------------------
// -----------------------------------------------

vec3 calculateDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 viewDirection) {
	vec3 lightDirection = normalize(-light.direction);

	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * material.diffuse;

	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularPower = pow(max(dot(viewDirection, halfwayDirection), 0.0), (1 - material.roughness) * 256);
	vec3 specular = light.colour * specularPower * material.specular;

	float rimPower = 1.0 - max(dot(viewDirection, normal), 0.0);
	rimPower = smoothstep(0.6, 1.0, rimPower);
	vec3 rim = light.colour * rimPower * material.diffuse;

	return diffuse + specular + rim;
}

vec3 calculatePointLight(PointLight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection) {
	vec3 lightToPositionDifference = light.position - worldPosition;
	vec3 lightDirection = normalize(lightToPositionDifference);

	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * material.diffuse;

	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularPower = pow(max(dot(viewDirection, halfwayDirection), 0.0), (1 - material.roughness) * 256);
	vec3 specular = light.colour * specularPower * material.specular;

	float rimPower = 1.0 - max(dot(viewDirection, normal), 0.0);
	rimPower = smoothstep(0.6, 1.0, rimPower);
	vec3 rim = light.colour * rimPower * material.diffuse;

	float dist = length(lightToPositionDifference);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(dist, 2.0));

	return attenuation * (diffuse + specular + rim);
}

vec3 calculateSpotlight(Spotlight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection) {
	vec3 lightToPositionDifference = light.position - worldPosition;
	vec3 lightDirection = normalize(lightToPositionDifference);

	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * material.diffuse;

	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularPower = pow(max(dot(viewDirection, halfwayDirection), 0.0), (1 - material.roughness) * 256);
	vec3 specular = light.colour * specularPower * material.specular;

	float rimPower = 1.0 - max(dot(viewDirection, normal), 0.0);
	rimPower = smoothstep(0.6, 1.0, rimPower);
	vec3 rim = light.colour * rimPower * material.diffuse;

	float dist = length(lightToPositionDifference);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(dist, 2.0));

	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.innerCutoffAngle - light.outerCutoffAngle;
	float intensity = clamp((theta - light.outerCutoffAngle) / epsilon, 0.0, 1.0);

	return intensity * attenuation * (diffuse + specular + rim);
}