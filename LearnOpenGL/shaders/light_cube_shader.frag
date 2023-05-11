#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D boxTexture;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0f);
//    FragColor = vec4(0.2f, 0.5f, 0.6f, 1.0f);
};