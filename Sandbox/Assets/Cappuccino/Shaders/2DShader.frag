#version 460

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

uniform sampler2D uTextureSlot;
uniform vec4 uColour;
uniform float uTileFactor;

void main() {
	vec4 colour = texture(uTextureSlot, inUV * uTileFactor) * uColour;
	if(colour.a <= 0.1) {
		discard;
	}
	outColour = colour;
}