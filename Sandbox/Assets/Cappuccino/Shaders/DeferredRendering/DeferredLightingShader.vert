#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out DATA {
	vec2 uv;
} outVert;

void main() {
	outVert.uv = inUV;
	gl_Position = vec4(inPosition, 1.0);
}