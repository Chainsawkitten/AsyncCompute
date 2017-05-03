/*
    Red fragment shader.
*/
#version 430

layout(location = 0) out vec4 fragmentColor;

layout(set=1, binding=0) uniform sampler2D particleTexture;

void main() {
    fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
}
