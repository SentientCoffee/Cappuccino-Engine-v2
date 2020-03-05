#version 460

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec2 inScreenCoords;

layout(location = 0) out vec4 outColor;

uniform sampler2D uImage;

void main() {
    outColor = vec4(1.0 - texture(uImage, inUV).xyz, 1.0);
}