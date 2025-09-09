#version 330 core

in vec3 ourColor;
in vec3 sinePos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(sinePos, 1.0);
};
