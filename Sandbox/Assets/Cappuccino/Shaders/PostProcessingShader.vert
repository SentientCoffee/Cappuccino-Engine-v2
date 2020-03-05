#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec2 outScreenCoords;

uniform vec2 uScreenSize;

void main() {
	outUV = inUV;
	outScreenCoords = ((inPosition.xy + vec2(1, 1)) / 2.0f) * uScreenSize;
	gl_Position = vec4(inPosition.xy, 0.0, 1.0);
}