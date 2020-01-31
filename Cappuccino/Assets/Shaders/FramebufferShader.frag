#version 460

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

uniform sampler2D uTextureSlot;

void main() {
	outColour = texture(uTextureSlot, inUV);
}