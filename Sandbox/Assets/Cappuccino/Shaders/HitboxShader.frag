#version 460

layout(location = 0) out vec4 outColour;

uniform vec4 uHitboxColour;

void main() {
	outColour = uHitboxColour;
}