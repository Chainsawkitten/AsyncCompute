/*
    Default empty shader.
*/
#version 430

in vec2 outTexCoord;

out vec4 out_color;

uniform sampler2D fragTexture;

void main() {
    out_color = vec4(1.0, 0.0, 0.0, 1.0);//texture(fragTexture, outTexCoord);
}