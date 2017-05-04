/*
    Red fragment shader.
*/
#version 430

in VertexData {
    vec4 velocity;
    vec2 texCoords;
} vertexIn;

layout(set = 2, binding = 0) uniform sampler2D particleTexture;

layout(location = 0) out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(vertexIn.velocity.xyz, 1.0) * texture(particleTexture, vertexIn.texCoords);
}
