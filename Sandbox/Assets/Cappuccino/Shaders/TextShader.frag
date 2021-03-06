#version 460

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColour;

uniform sampler2D uTextureSlot;
uniform vec4 uTextColour;

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTextureSlot, inUV).r);
	outColour = uTextColour * sampled;
}