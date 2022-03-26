#version 300 es

precision mediump float;

layout (location = 0) in highp vec2 in_position;
layout (location = 1) in vec4 in_colour;
layout (location = 2) in vec2 in_textureCoordinates;

out vec4 v_colour;
out vec2 v_textureCoordinates;

uniform mat4 u_ViewProjection;

void main()
{
    v_colour = in_colour;
    v_textureCoordinates = in_textureCoordinates;

    gl_Position = u_ViewProjection * vec4(in_position, 0.0, 1.0);
}
