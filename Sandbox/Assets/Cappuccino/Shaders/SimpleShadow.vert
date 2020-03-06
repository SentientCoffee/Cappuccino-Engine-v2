#version 460

layout(location = 0) in vec3 inPosition;

uniform mat4 uTransform;
uniform mat4 uViewProjection;

void main() {
	gl_Position = uViewProjection * uTransform * vec4(inPosition, 1.0);
}