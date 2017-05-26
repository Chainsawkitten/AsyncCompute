/*
    Particle geometry shader (expands particles).
*/
#version 430

// Input.
layout(points) in;
layout(triangle_strip, max_vertices=4) out;

layout(location = 0) in vec4 velocityIn[1];

layout(set = 1, binding = 0) uniform CameraUniform {
    mat4 viewProjectionMatrix;
    vec4 cameraPosition;
    vec4 cameraUp;
};

layout(location = 0) out vec4 velocityOut;
layout(location = 1) out vec2 texCoordsOut;

void main() {
    vec2 size = vec2(0.4, 0.4);
    
    // Vector from camera to point, cameras position extracted from view matrix
    vec4 look = vec4(cameraPosition.xyz - gl_in[0].gl_Position.xyz, 0.0);
    
    // Particle right vector = camera up x look
    vec4 right = normalize(vec4(cross(cameraUp.xyz, look.xyz), 0.0)) * size.x * 0.5;
    
    // Particle up vector = particle right x look
    vec4 up = normalize(vec4(cross(look.xyz, right.xyz), 0.0)) * size.y * 0.5;
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position - right + up);
    velocityOut = velocityIn[0];
    texCoordsOut = vec2(0.0, 0.0);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position - right - up);
    velocityOut = velocityIn[0];
    texCoordsOut = vec2(1.0, 0.0);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position + right + up);
    velocityOut = velocityIn[0];
    texCoordsOut = vec2(0.0, 1.0);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position + right - up);
    velocityOut = velocityIn[0];
    texCoordsOut = vec2(1.0, 1.0);
    EmitVertex();
    
    EndPrimitive();
}
