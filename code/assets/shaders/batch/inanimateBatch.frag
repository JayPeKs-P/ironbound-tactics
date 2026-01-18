#version 460 core

out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
    vec4 rawTexture = texture(ourTexture, TexCoord);
    if (rawTexture.a < 0.1){
        discard;
    }
    fragColor = rawTexture;
}
