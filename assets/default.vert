/*
    Default empty shader.
*/
#version 450
#extension GL_KHR_vulkan_glsl : enable

struct vertex {
	vec4 position;
	vec2 texCoord;
};

layout(std430, binding = 0) buffer vertexBuffer{
    vertex vertices[];
};

out vec2 outTexCoord;

void main() {
    vertex vertices2[3];
    vertices2[0].position = vec4(0.0, 0.0, 0.0, 1.0);
    vertices2[1].position = vec4(-0.5, -0.5, 0.0, 1.0);
    vertices2[2].position = vec4(0.5, -0.5, 0.0, 1.0);
    
	gl_Position = vertices2[gl_VertexIndex].position;
	outTexCoord = vertices2[gl_VertexIndex].texCoord;
}