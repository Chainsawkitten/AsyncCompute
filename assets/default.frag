/*
    Red fragment shader.
*/
#version 430

layout(location = 0) in vec4 velocityIn;
layout(location = 1) in vec2 texCoordsIn;

layout(set = 2, binding = 0) uniform sampler2D particleTexture;

layout(location = 0) out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(velocityIn.xyz, 1.0) * texture(particleTexture, texCoordsIn);
}
