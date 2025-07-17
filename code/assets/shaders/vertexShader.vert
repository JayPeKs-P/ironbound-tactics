#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 instanceModel;

out vec3 ourColor;
out vec2 TexCoord;

uniform float uvOffset;

void main() {
    gl_Position = instanceModel * vec4(aPosition, 1.0f);
    ourColor = aColor;
    TexCoord = aTexCoord + vec2(uvOffset, 0.0f);
}