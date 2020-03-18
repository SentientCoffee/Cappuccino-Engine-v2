#version 460

layout(location = 0) in vec4 inPosition;

uniform vec3 uLightPosition;
uniform float uFarPlane;

void main() {
	float distanceToLight = length(inPosition.xyz - uLightPosition);
	distanceToLight /= uFarPlane;

	gl_FragDepth = distanceToLight;
}