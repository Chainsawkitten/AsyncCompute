/*
    Default empty shader.
*/
#version 430

struct Particle {
    vec4 position;
    vec4 velocity;
};

layout(set = 0, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

out VertexData {
    vec4 velocity;
} vertexOut;

void main() {
    gl_Position = particles[gl_VertexIndex].position;
    vertexOut.velocity = particles[gl_VertexIndex].velocity;
}
