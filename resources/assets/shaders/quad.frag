#version 300 es

precision mediump float;

in vec4 v_colour;
in vec2 v_textureCoordinates;

layout (location = 0) out vec4 out_colour;

uniform sampler2D u_Texture;

void main()
{
    out_colour = texture(u_Texture, v_textureCoordinates) * v_colour;
}
