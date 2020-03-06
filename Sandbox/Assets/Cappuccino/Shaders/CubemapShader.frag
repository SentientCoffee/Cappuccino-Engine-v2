#version 460

layout (location = 0) in vec3 inUV;

layout (location = 0) out vec4 outColour;

uniform samplerCube uSkybox;
uniform float uGamma = 2.2;

void main() {
	outColour = pow(texture(uSkybox, normalize(inUV)), vec4(uGamma));
}