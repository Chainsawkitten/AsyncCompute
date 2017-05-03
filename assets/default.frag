/*
    Default empty shader.
*/
#version 430

in vec2 outTexCoord;

out vec4 outColor;

uniform sampler2D fragTexture;

void main() {
    outColor = vec4(1.0, 0.0, 0.0, 1.0);//texture(fragTexture, outTexCoord);
}