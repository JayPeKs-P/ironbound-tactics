#version 460 core

in vec3 normal;
uniform vec4 color = vec4(0.945f, 0.768f, 0.058f, 1.0f);
uniform vec4 lightDirection = vec4(-0.5f, -0.5f, -0.5f, 1.0f);
out vec4 fragColor;

void main() {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(vec3(lightDirection));
    float attenuation = max(dot(norm, -lightDir), 0.0f);
    fragColor = attenuation * color;
}