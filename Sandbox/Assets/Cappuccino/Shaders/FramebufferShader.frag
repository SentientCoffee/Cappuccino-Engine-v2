#version 460

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

uniform sampler2D uTextureSlot;

void main() {
	vec4 colour = texture(uTextureSlot, inUV);
	if(colour.a < 0.1) {
		discard;
	}
	outColour = colour;
}