#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out DATA {
	vec3 viewPosition;
	vec2 uv;
	vec3 normal;
} outVert;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

void main() {
	vec4 viewPosition = uView * uTransform * vec4(inPosition, 1.0);
	
	outVert.viewPosition = viewPosition.xyz;
	outVert.uv = inUV;
	outVert.normal = uNormalMatrix * inNormal;

	gl_Position = uProjection * viewPosition;
}