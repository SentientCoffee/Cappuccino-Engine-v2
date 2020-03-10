#version 460

layout(location = 0) in vec3 inPosition;

uniform mat4 uTransform;

void main() {
	gl_Position = uTransform * vec4(inPosition, 1.0);
}