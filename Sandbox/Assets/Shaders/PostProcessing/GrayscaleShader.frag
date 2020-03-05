#version 460

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec2 inScreenCoords;

layout(location = 0) out vec4 outColor;

uniform sampler2D uImage;

void main() {
	vec4 fragColor = texture(uImage, inUV);
    float average = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
    outColor = vec4(average, average, average, 1.0);
}