#version 300 es

precision mediump float;

layout (location = 0) in highp vec2 in_position;
layout (location = 1) in vec4 in_colour;

out vec4 v_colour;

uniform mat4 u_ViewProjection;

void main()
{
    v_colour = in_colour;

    gl_Position = u_ViewProjection * vec4(in_position, 0.0, 1.0);
}
