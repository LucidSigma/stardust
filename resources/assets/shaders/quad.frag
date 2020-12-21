#version 460 core

layout (location = 0) out vec4 out_colour;

uniform vec4 u_Colour;

void main()
{
	out_colour = u_Colour;
}