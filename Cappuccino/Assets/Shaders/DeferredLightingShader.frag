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

	mat4 view;
};

struct PointLight {
	vec3 position;
	vec3 colour;
	float attenuation;

	mat4 view;
};

struct Spotlight {
	vec3 position;
	vec3 direction;
	vec3 colour;
	float attenuation;
	float innerCutoffAngle;
	float outerCutoffAngle;

	mat4 view;
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
	sampler2D normal;
	sampler2D albedo;
	sampler2D specRough;
	sampler2D emission;
};

struct DepthTextures {
	sampler2D camera;
	sampler2D lightShadow;
};

// -----------------------------------------------
// ----- Uniforms --------------------------------
// -----------------------------------------------

uniform vec3 uAmbientColour;
uniform float uAmbientPower;

uniform int uNumDirectionalLights;
uniform int uNumPointLights;
uniform int uNumSpotlights;
uniform DirectionalLight uDirectionalLights[MAX_DIR_LIGHTS];
uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform Spotlight uSpotlights[MAX_SPOTLIGHTS];

uniform float uShadowBias = 0.01;

uniform GBuffer uGBuffer;
uniform DepthTextures uDepths;
uniform mat4 uViewProjectionInverse;

// -----------------------------------------------
// ----- Functions -------------------------------
// -----------------------------------------------

float calculateShadowPCF(vec3 worldPosition, float bias);
vec3 calculateDirectionalLight(DirectionalLight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection, float shadow);
vec3 calculatePointLight(PointLight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection, float shadow);
vec3 calculateSpotlight(Spotlight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection, float shadow);

// -----------------------------------------------
// ----- MAIN ------------------------------------
// -----------------------------------------------

void main() {
	// Get world position from clip space
	float zOverW = texture(uDepths.camera, inFrag.uv).r * 2.0 - 1.0;
	vec4 clipPosition = vec4((inFrag.uv * 2.0 - 1.0), zOverW, 1.0);
	vec4 worldPosition = uViewProjectionInverse * clipPosition;
	worldPosition /= worldPosition.w;

	// Shadow component
	vec4 shadowPosition = light.view * worldPosition;
	shadowPosition /= shadowPosition.w;
	shadowPosition = shadowPosition * 0.5 + 0.5;

	float bias = max(uShadowBias * 10.0 * (1.0 - dot(normal, lightDirection)), uShadowBias);
	float shadow = calculateShadowPCF(shadowPosition.xyz, bias);

	if(shadowPosition.x < 0 || shadowPosition.x > 1 || shadowPosition.y < 0 || shadowPosition.y > 1 || shadowPosition.z < 0 || shadowPosition.z > 1) {
		shadow = 0.0;
	}

	vec3 normal = texture(uGBuffer.normal, inFrag.uv).rgb;
	
	mat.diffuse   = texture(uGBuffer.albedo,    inFrag.uv).rgb;
	mat.emission  = texture(uGBuffer.emission,  inFrag.uv).rgb;
	mat.specular  = texture(uGBuffer.specRough, inFrag.uv).rrr;
	mat.roughness = texture(uGBuffer.specRough, inFrag.uv).g;

	vec3 viewDirection = normalize(-worldPosition);
	vec3 result = uAmbientColour * uAmbientPower * mat.diffuse;

	for(int i = 0; i < uNumDirectionalLights; ++i) {
		if(i >= MAX_DIR_LIGHTS) break;
		result += calculateDirectionalLight(uDirectionalLights[i], mat, worldPosition, normal, viewDirection, shadow);
	}

	for(int i = 0; i < uNumPointLights; ++i) {
		if(i >= MAX_POINT_LIGHTS) break;
		result += calculatePointLight(uPointLights[i], mat, worldPosition, normal, viewDirection, shadow);
	}

	for(int i = 0; i < uNumSpotlights; ++i) {
		if(i >= MAX_SPOTLIGHTS) break;
		result += calculateSpotlight(uSpotlights[i], mat, worldPosition, normal, viewDirection, shadow);
	}

	result += mat.emission;
	outColour = vec4(result, 1.0);
}

// -----------------------------------------------
// ----- Function defintions ---------------------
// -----------------------------------------------

float calculateShadowPCF(vec3 worldPosition, float bias) {
	float result = 0.0;
	vec2 texelSize = 1.0 / textureSize(uDepths.lightShadow, 0.0);

	for(int x = -1; x <= 1; ++x) { 
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(uDepths.lightShadow, fragPos.xy + vec2(x, y) * texelSize).r;
			result += fragPos.z - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	result /= 9.0; // Average our sum
	return result;
}

vec3 calculateDirectionalLight(DirectionalLight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection, float shadow) {
	vec3 lightDirection = normalize(-light.direction);

	// Diffuse component
	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * material.diffuse;

	// Specular component
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularPower = pow(max(dot(viewDirection, halfwayDirection), 0.0), (1 - material.roughness) * 256);
	vec3 specular = light.colour * specularPower * material.specular;

	// Rim component
	float rimPower = 1.0 - max(dot(viewDirection, normal), 0.0);
	rimPower = smoothstep(0.6, 1.0, rimPower);
	vec3 rim = light.colour * rimPower * material.diffuse;

	return (1.0 - shadow) * (diffuse + specular + rim);
}

vec3 calculatePointLight(PointLight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection, float shadow) {
	vec3 lightToPositionDifference = light.position - worldPosition;
	vec3 lightDirection = normalize(lightToPositionDifference);
	
	// Diffuse component
	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * material.diffuse;

	// Specular component
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularPower = pow(max(dot(viewDirection, halfwayDirection), 0.0), (1 - material.roughness) * 256);
	vec3 specular = light.colour * specularPower * material.specular;

	// Rim component
	float rimPower = 1.0 - max(dot(viewDirection, normal), 0.0);
	rimPower = smoothstep(0.6, 1.0, rimPower);
	vec3 rim = light.colour * rimPower * material.diffuse;

	// Attenuation component
	float dist = length(lightToPositionDifference);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(dist, 2.0));

	return (1.0 - shadow) * attenuation * (diffuse + specular + rim);
}

vec3 calculateSpotlight(Spotlight light, Material material, vec3 worldPosition, vec3 normal, vec3 viewDirection, float shadow) {
	vec3 lightToPositionDifference = light.position - worldPosition;
	vec3 lightDirection = normalize(lightToPositionDifference);

	// Diffuse component
	float diffusePower = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.colour * diffusePower * material.diffuse;

	// Specular component
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularPower = pow(max(dot(viewDirection, halfwayDirection), 0.0), (1 - material.roughness) * 256);
	vec3 specular = light.colour * specularPower * material.specular;

	// Rim component
	float rimPower = 1.0 - max(dot(viewDirection, normal), 0.0);
	rimPower = smoothstep(0.6, 1.0, rimPower);
	vec3 rim = light.colour * rimPower * material.diffuse;

	// Attenuation
	float dist = length(lightToPositionDifference);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(dist, 2.0));

	// Intensity component
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.innerCutoffAngle - light.outerCutoffAngle;
	float intensity = clamp((theta - light.outerCutoffAngle) / epsilon, 0.0, 1.0);

	return (1.0 - shadow) * intensity * attenuation * (diffuse + specular + rim);
}