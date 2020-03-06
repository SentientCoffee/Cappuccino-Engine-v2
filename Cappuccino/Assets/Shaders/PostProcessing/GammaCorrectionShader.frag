#version 460

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec2 inScreenCoords;

layout(location = 0) out vec4 outColor;

uniform sampler2D uImage;
uniform float uGamma = 2.2;

void main() {
	outColor = pow(texture(uImage, inUV), vec4(1.0 / uGamma));
}