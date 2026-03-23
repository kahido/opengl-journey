#version 460 core

layout(location = 0) out vec4 out_color;

in vec3 v_color;
in vec3 v_position;

uniform float u_time;

void main()
{
    out_color = vec4( (sin(u_time + v_color) + 1)/2, 1);
}
