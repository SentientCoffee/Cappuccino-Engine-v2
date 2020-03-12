#version 460

layout(location = 0) in vec4 inPosition;

uniform vec3 uLightPosition;
uniform float uFarPlane;

void main() {
	float distancetoLight = length(inPosition.xyz - uLightPosition);
	distancetoLight /= uFarPlane;

	gl_FragDepth = distancetoLight;
}