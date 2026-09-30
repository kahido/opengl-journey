#version 460 core

layout(location = 0) out vec4 out_color;

in vec3 v_color;
in vec2 v_tex_position;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    // out_color = vec4(v_color, 1);
    // out_color = texture(ourTexture, v_tex_position) * vec4(v_color, 1);
    out_color = mix(texture(ourTexture1, v_tex_position), texture(ourTexture2, v_tex_position), 0.2);
}
