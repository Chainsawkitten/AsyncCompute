/*
    Red fragment shader.
*/
#version 430

in VertexData {
    vec2 texCoords;
} vertexIn;

layout(set=1, binding=0) uniform sampler2D particleTexture;

layout(location = 0) out vec4 fragmentColor;

void main() {
    fragmentColor = texture(particleTexture, vertexIn.texCoords);
}
