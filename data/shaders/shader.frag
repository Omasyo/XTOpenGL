#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
    FragColor = vec4(lightColor * objectColor, 1.0);
    // FragColor = texture(ourTexture, TexCoord);
}