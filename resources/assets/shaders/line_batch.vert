#version 300 es

precision mediump float;

layout (location = 0) in highp vec2 in_position;
layout (location = 1) in vec4 in_colour;
layout (location = 2) in float in_projectionType;

out vec4 v_colour;

uniform mat4 u_ViewProjection;
uniform mat4 u_ScreenProjection;

void main()
{
    v_colour = in_colour;

    if (in_projectionType < 0.5)
    {
        gl_Position = u_ViewProjection * vec4(in_position, 0.0, 1.0);
    }
    else
    {
        gl_Position = u_ScreenProjection * vec4(in_position, 0.0, 1.0);
    }
}
