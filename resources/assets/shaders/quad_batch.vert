#version 300 es

precision mediump float;

layout (location = 0) in highp vec2 in_position;
layout (location = 1) in vec4 in_colour;
layout (location = 2) in vec2 in_textureCoordinates;
layout (location = 3) in float in_textureIndex;
layout (location = 4) in float in_projectionType;

out vec4 v_colour;
out vec2 v_textureCoordinates;
out float v_textureIndex;

uniform mat4 u_ViewProjection;
uniform mat4 u_ScreenProjection;

void main()
{
    v_colour = in_colour;
    v_textureCoordinates = in_textureCoordinates;
    v_textureIndex = in_textureIndex;

    if (in_projectionType < 0.5)
    {
        gl_Position = u_ViewProjection * vec4(in_position, 0.0, 1.0);
    }
    else
    {
        gl_Position = u_ScreenProjection * vec4(in_position, 0.0, 1.0);
    }
}
