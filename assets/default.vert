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

layout(location = 0) out vec4 velocityOut;

void main() {
    gl_Position = particles[gl_VertexIndex].position;
    velocityOut = particles[gl_VertexIndex].velocity;
}
