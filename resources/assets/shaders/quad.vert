#version 460 core

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_unused;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(in_position, 0.0, 1.0);
}