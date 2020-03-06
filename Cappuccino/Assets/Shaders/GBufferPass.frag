#version 460

// -----------------------------------------------
// ----- Input values ----------------------------
// -----------------------------------------------

layout(location = 0) in DATA {
	vec3 worldPosition;
	vec2 uv;
	vec3 normal;
} inFrag;

// -----------------------------------------------
// ----- GBuffer output targets ------------------
// -----------------------------------------------

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outAlbedo;
layout(location = 3) out vec4 outSpecRough;
layout(location = 4) out vec4 outEmission;

// -----------------------------------------------
// ----- Material struct -------------------------
// -----------------------------------------------

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	sampler2D emission;
	sampler2D bump;
	float roughness;
};

uniform Material uMaterial;
uniform float uGamma = 2.2;

// -----------------------------------------------
// ----- MAIN ------------------------------------
// -----------------------------------------------

void main() {
	outPosition     = vec4(inFrag.worldPosition, 1.0);
	outNormal       = vec4((normalize(inFrag.normal) * 0.5 + vec3(0.5)), 1.0);
	outAlbedo       = pow(texture(uMaterial.diffuse, inFrag.uv), vec4(uGamma));
	outEmission     = pow(texture(uMaterial.emission, inFrag.uv), vec4(uGamma));
	outSpecRough.r  = texture(uMaterial.specular, inFrag.uv).r;
	outSpecRough.g  = uMaterial.roughness;
	outSpecRough.ba = vec2(1.0);
}