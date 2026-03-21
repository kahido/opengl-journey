#version 460 core

layout(location = 0) out vec4 out_color;

in vec3 v_color;
in vec3 v_position;

void main()
{
    vec3 new_color = v_position;
    new_color += 1;
    new_color /= 2;
    out_color = vec4(new_color, 1);
}
