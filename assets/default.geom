/*
    Particle geometry shader (expands particles).
*/
#version 430

// Input.
layout(points) in;
layout(triangle_strip, max_vertices=4) out;

layout(set = 0, binding = 0) uniform CameraUniform {
    mat4 viewProjectionMatrix;
};

void main() {
    vec2 size = vec2(0.1, 0.1);
    
    // Particle right vector = camera up x look
    vec4 right = vec4(1.0, 0.0, 0.0, 0.0) * size.x * 0.5;
    
    // Particle up vector = particle right x look
    vec4 up = vec4(0.0, 1.0, 0.0, 0.0) * size.y * 0.5;
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position - right + up);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position - right - up);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position + right + up);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position + right - up);
    EmitVertex();
    
    EndPrimitive();
}
