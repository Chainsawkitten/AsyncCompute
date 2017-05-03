/*
    Default empty shader.
*/
#version 430

layout(set = 0, binding = 0) buffer ParticleBuffer {
    vec4 positions[];
};

void main() {
    gl_Position = positions[gl_VertexIndex];
}
