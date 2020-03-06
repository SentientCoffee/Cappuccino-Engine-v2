#version 460

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec2 inScreenCoords;

layout(location = 0) out vec4 outColor;

struct Buffers {
	sampler2D uCameraColor;
	sampler2D uCameraDepth;

void main() {
	outColor = pow(texture(uImage, inUV), vec4(1.0 / uGamma));
}