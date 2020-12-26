#version 460 core

layout (location = 0) in vec4 v_colour;

layout (location = 0) out vec4 out_colour;

void main()
{
	out_colour = v_colour;
}