#version 460 core

layout(location = 0) in vec3 is_position;
layout(location = 1) in vec3 is_color;

void main()
{
    gl_Position = vec4(is_position, 1);
}
