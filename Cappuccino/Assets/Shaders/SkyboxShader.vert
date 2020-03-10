#version 460

layout (location = 0) in vec3 inPosition;

layout (location = 0) out vec3 outUV;

uniform mat4 uViewProjection;

void main() {
	outUV = normalize(inPosition);
	vec4 outPosition = uViewProjection * vec4(inPosition, 1.0);
	gl_Position = outPosition.xyww;
}