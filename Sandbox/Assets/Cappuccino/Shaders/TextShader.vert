#version 460

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;

uniform mat4 uProjection;

void main() {
	outUV = inUV;
	gl_Position = uProjection * vec4(inPosition, 0.0, 1.0);
}