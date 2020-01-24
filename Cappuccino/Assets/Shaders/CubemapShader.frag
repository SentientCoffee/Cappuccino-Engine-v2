#version 460

layout (location = 0) in vec3 inUV;

layout (location = 0) out vec4 outColour;

uniform samplerCube uSkybox;

void main() {
	outColour = texture(uSkybox, normalize(inUV));
}