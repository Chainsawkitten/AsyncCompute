/*
    Default empty shader.
*/
#version 430

void main() {
    vec4 positions[3];
    positions[0] = vec4(0.0, 0.0, 0.0, 1.0);
    positions[1] = vec4(0.5, 0.5, 0.0, 1.0);
    positions[2] = vec4(-0.5, 0.5, 0.0, 1.0);
    
    gl_Position = positions[gl_VertexIndex];
}
