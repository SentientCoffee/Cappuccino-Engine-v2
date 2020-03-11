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
// ----- Structs ---------------------------------
// -----------------------------------------------

// Spotlight values
struct Spotlight {
	vec3 position;
	vec3 direction;
	vec3 colour;
	float attenuation;
	float innerCutoffAngle;
	float outerCutoffAngle;
};

// Pseudo material (for lighting)
struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 normal;
	vec3 emission;
	vec3 bump;
	float roughness;
} material;

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

uniform vec3 uAmbientColour;
uniform float uAmbientPower;

uniform Spotlight uSpotlight;
uniform mat4 uLightViewSpace;

uniform sampler2D uShadowMap;
uniform float uShadowBias = 0.001;

uniform GBuffer uGBuffer;

// -----------------------------------------------
// ----- Functions -------------------------------
// -----------------------------------------------

float calculateProjShadowPCF(vec4 viewPosition, float bias);
vec3 calculateSpotlight(Spotlight light, Material material, vec4 viewPosition, vec3 normal);

// -----------------------------------------------
// ----- MAIN ------------------------------------
// -----------------------------------------------

void main() {
	// Get world position from clip space
	vec4 viewPosition = vec4(texture(uGBuffer.position, inFrag.uv).rgb, 1.0);
	if(length(viewPosition) == 0.0) {
		discard;
	}

	vec3 normal = (texture(uGBuffer.normal, inFrag.uv).rgb - vec3(0.5)) * 2.0;
	
	material.diffuse   = texture(uGBuffer.albedo,    inFrag.uv).rgb;
	material.emission  = texture(uGBuffer.emission,  inFrag.uv).rgb;
	material.specular  = texture(uGBuffer.specRough, inFrag.uv).rrr;
	material.roughness = texture(uGBuffer.specRough, inFrag.uv).g;

	vec3 result = uAmbientColour * uAmbientPower * material.diffuse;
	result += calculateSpotlight(uSpotlight, material, viewPosition, normal);
	result += material.emission;

	outColour = vec4(result, 1.0);
}

// -----------------------------------------------
// ----- Function defintions ---------------------
// -----------------------------------------------

float calculateProjShadowPCF(vec4 shadowPosition, float bias) {
	float result = 0.0;
	vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);

	for(int x = -1; x <= 1; ++x) { 
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(uShadowMap, ((shadowPosition.xy + vec2(x, y)) / shadowPosition.w) * texelSize).r;
			result += ((shadowPosition.z - bias) / shadowPosition.w) > pcfDepth ? 1.0 : 0.0;
		}
	}

	result /= 9.0; // Average our sum
	return result;
}

vec3 calculateSpotlight(Spotlight light, Material material, vec4 viewPosition, vec3 normal) {
	vec3 viewDirection = normalize(-viewPosition.xyz);
	vec3 lightToPositionDifference = light.position - viewPosition.xyz;
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

	// Shadow component
	vec4 shadowPosition = uLightViewSpace * viewPosition;
	shadowPosition /= shadowPosition.w;
	shadowPosition = shadowPosition * 0.5 + 0.5;

	float bias = max(uShadowBias * 10.0 * (1.0 - dot(normal, lightDirection)), uShadowBias);
	float shadowDepth = texture(uShadowMap, shadowPosition.xy / shadowPosition.w).r;
	float shadow = shadowDepth < ((shadowPosition.z - bias) / shadowPosition.w) ? 1.0 : 0.0;
	//float shadow = calculateProjShadowPCF(shadowPosition, bias);

	if(shadowPosition.x < 0 || shadowPosition.x > 1 || shadowPosition.y < 0 || shadowPosition.y > 1 || shadowPosition.z < 0 || shadowPosition.z > 1) {
		shadow = 0.0;
	}

	return (1.0 - shadow) * intensity * attenuation * (diffuse + specular + rim);
}