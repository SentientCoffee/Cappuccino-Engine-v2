#version 460

// -----------------------------------------------
// ----- Input values ----------------------------
// -----------------------------------------------

in DATA {
	vec3 worldPosition;
	vec2 uv;
	vec3 normal;
} inFrag;

// -----------------------------------------------
// ----- GBuffer output targets ------------------
// -----------------------------------------------

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outAlbedo;
layout(location = 3) out vec4 outSpecRough;
layout(location = 4) out vec3 outEmission;

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

// -----------------------------------------------
// ----- Uniforms --------------------------------
// -----------------------------------------------

uniform Material uMaterial;

// -----------------------------------------------
// ----- MAIN ------------------------------------
// -----------------------------------------------

void main() {
	outPosition      = inFrag.worldPosition;
	outNormal        = normalize(inFrag.normal);
	outAlbedo        = texture(uMaterial.diffuse, inFrag.uv).rgb;
	outEmission      = texture(uMaterial.emission, inFrag.uv).rgb;
	outSpecRough.rgb = texture(uMaterial.specular, inFrag.uv).rgb;
	outSpecRough.a   = uMaterial.roughness;
}