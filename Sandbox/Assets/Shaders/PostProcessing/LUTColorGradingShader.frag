#version 460

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec2 inScreenCoords;

layout(location = 0) out vec4 outColor;

uniform sampler2D uImage;

uniform sampler3D uLUTSlot;
uniform vec3 uLUTDimensions;

void main() {
	vec3 colour = texture(uImage, inUV).rgb;
	vec3 scale = (uLUTDimensions - 1.0) / uLUTDimensions;
	vec3 offset = 1.0 / (2.0 * uLUTDimensions);

	outColor.rgb = texture(uLUTSlot, scale * colour + offset).rgb;
	outColor.a = 1.0;
}