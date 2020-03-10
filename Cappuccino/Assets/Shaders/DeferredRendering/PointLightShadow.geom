#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

out vec4 lightViewPosition;

uniform mat4 uLightViews[6];

void main() {
	for(int face = 0; face < 6; ++face) {
		gl_Layer = face;
		for(int i = 0; i < 3; ++i) {
			// World space from vertex shader
			lightViewPosition = gl_in[i].gl_Position;
			// Convert from camera view space to light view space
			gl_Position = uLightViews[face] * lightViewPosition;
			EmitVertex();
		}
		EndPrimitive();
	}
}