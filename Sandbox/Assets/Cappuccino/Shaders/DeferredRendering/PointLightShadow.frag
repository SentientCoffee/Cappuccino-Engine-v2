#version 460

in vec4 lightViewPosition;

uniform vec3 uLightPosition;
uniform float uFarPlane;

void main() {
	float distancetoLight = length(lightViewPosition.xyz - uLightPosition);
	distancetoLight /= uFarPlane;

	gl_FragDepth = distancetoLight;
}