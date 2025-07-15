#version 460 core

uniform vec4 color = vec4(0.945f, 0.768f, 0.058f, 1.0f);
out vec4 fragColor;

void main() {
    fragColor = color;
}