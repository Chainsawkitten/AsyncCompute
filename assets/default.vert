/*
    Default empty shader.
*/
#version 450
layout(std430, binding = 1) buffer vertexBuffer{
	vec3 position;
	vec3 color;
	vec2 texCoord;
};

out vec3 outColor;
out vec2 outTexCoord;

void main() {
	gl_Position = vec4(position, 1.0f);
	outColor = color;
	outTexCoord = texCoord;

}