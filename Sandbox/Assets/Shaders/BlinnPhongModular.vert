#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec2 outUV;
layout(location = 2) out vec3 outNormal;

uniform mat4 uViewProjection;
uniform mat4 uTransform;
uniform mat3 uNormalMatrix;

void main() {
	outWorldPosition = (uTransform * vec4(inPosition, 1.0)).xyz;
	outUV = inUV;
	outNormal = uNormalMatrix * inNormal;
	
	gl_Position = uViewProjection * uTransform * vec4(inPosition, 1.0);
}