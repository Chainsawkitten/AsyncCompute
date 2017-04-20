/*
    Default empty shader.
*/
#version 450

in vec3 outColor;
in vec2 outTexCoord;

out vec4 out_color;

uniform sampler2D fragTexture;

void main() {
    out_color = texture(fragTexture, outTexCoord);
}