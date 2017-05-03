/*
    Red fragment shader.
*/
#version 430

layout(location = 0) out vec4 fragmentColor;

layout(set=1, binding=0) uniform sampler2D particleTexture;

void main() {
    fragmentColor = texture(particleTexture, vec2(0.5, 0.5));
}
