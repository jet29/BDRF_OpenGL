#version 330 core

uniform vec3 colorIn;
out vec4 color;

void main()
{
    color = vec4(colorIn, 1);
}