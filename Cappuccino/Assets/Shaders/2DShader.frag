#version 460

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

uniform sampler2D uTextureSlot;
uniform vec4 uColour;
uniform float uTileFactor;

void main() {
	outColour = texture(uTextureSlot, inUV * uTileFactor) * uColour;
}