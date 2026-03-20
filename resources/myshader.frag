#version 460 core

layout(location = 0) out vec4 out_color;

in vec3 v_color;

void main()
{
    // Ex. 1
    // out_color = vec4(v_color.r, v_color.r, v_color.r, 1);

    // Ex. 2
    // out_color = vec4(v_color.r, v_color.g, v_color.g, 1);

    // 1 0 0
    // 0 1 1
    // 0 0 0

    // Ex. 3
    // out_color = vec4(v_color.g + v_color.b, v_color.b + v_color.r, v_color.r + v_color.g, 1);

    // Ex. 4
    // out_color = vec4(v_color.r, v_color.g, v_color.b, 1);

    // if (out_color.r > out_color.g + out_color.b)
    // {
    //     out_color.r = 1;
    //     out_color.g = 0;
    //     out_color.b = 0;
    // }

    // Ex. 5

    // if (int(v_color.r * 10) % 2 == 0)
    // {
    //     out_color = vec4(1, 0, 0, 1);
    // }
    // else
    // {
    //     out_color = vec4(1, 1, 1, 1);
    // }

    // if (v_color.b == 1 ) out_color.rgb = 1;

    // out_color.rgb *= 10;
    // out_color.rgb = floor(out_color.rgb);
    // out_color.rgb /= 10;
}
