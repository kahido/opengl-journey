#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_tex_position;

out vec3 v_color;
out vec2 v_tex_position;

void main()
{
    gl_Position = vec4(in_position, 1.0);
    // gl_Position.xyz *= 0.8;

    v_color = in_color;
    v_tex_position = in_tex_position;
}
