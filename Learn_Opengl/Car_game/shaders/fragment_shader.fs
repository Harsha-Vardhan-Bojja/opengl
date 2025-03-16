#version 330 core

in vec2 TexCoord; // Receive texture coordinate from vertex shader
out vec4 FragColor;

uniform sampler2D texture1; // Texture sampler

void main() {
    FragColor = texture(texture1, TexCoord); // Sample texture
}
