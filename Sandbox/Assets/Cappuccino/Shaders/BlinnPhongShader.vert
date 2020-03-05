#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out DATA {
	vec3 worldPosition;
	vec2 uv;
	vec3 normal;
} outVert;

uniform mat4 uViewProjection;
uniform mat4 uTransform;
uniform mat3 uNormalMatrix;

void main() {
	outVert.worldPosition = (uTransform * vec4(inPosition, 1.0)).xyz;
	outVert.uv = inUV;
	outVert.normal = uNormalMatrix * inNormal;
	
	gl_Position = uViewProjection * uTransform * vec4(inPosition, 1.0);
}