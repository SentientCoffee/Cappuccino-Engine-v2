#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

layout(location = 0) out vec4 outPosition;

uniform mat4 uShadowViewProjections[6];

void main() {
	for(int face = 0; face < 6; ++face) {
		gl_Layer = face;
		for(int i = 0; i < 3; ++i) {
			// World space from vertex shader
			outPosition = gl_in[i].gl_Position;
			// View projection multiplication
			gl_Position = uShadowViewProjections[face] * outPosition;
			EmitVertex();
		}
		EndPrimitive();
	}
}