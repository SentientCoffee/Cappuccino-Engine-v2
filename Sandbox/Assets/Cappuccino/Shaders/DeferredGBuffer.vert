#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

out DATA {
	vec3 worldPosition;
	vec2 uv;
	vec3 normal;
} outVert;

uniform mat4 uTransform;
uniform mat4 uViewProjection;
uniform mat3 uNormalMatrix;

void main() {
	vec4 position = uTransform * vec4(inPosition, 1.0);
	
	outVert.worldPosition = position.xyz;
	outVert.uv = inUV;
	outVert.normal = uNormalMatrix * inNormal;

	gl_Position = uViewProjection * position;
}