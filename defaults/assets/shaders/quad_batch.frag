#version 460 core

layout (location = 0) in vec4 v_colour;
layout (location = 1) in vec2 v_textureCoordinates;
layout (location = 2) in float v_textureIndex;

layout (location = 0) out vec4 out_colour;

uniform sampler2D u_Textures[32];

void main()
{
	const uint index = uint(v_textureIndex);
	out_colour = texture(u_Textures[index], v_textureCoordinates) * v_colour;
}