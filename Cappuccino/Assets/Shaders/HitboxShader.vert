#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

void main() {
	gl_Position = uViewProjection * uTransform * vec4(inPosition, 1.0);
}