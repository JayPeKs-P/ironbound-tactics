#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 instanceModel;
layout (location = 7) in vec2 aUvOffset;

out vec3 ourColor;
out vec2 TexCoord;


void main() {
    gl_Position = instanceModel * vec4(aPosition, 1.0f);
    ourColor = aColor;
    float tileSize = 1.0f / 4.0f;
    TexCoord = aTexCoord * tileSize + vec2(aUvOffset.x, 3.0 - aUvOffset.y) * tileSize;
}