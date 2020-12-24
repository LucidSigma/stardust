#version 460 core

layout (location = 0) in vec2 v_textureCoordinates;

layout (location = 0) out vec4 out_colour;

uniform sampler2D u_TextureSampler;
uniform vec4 u_ColourMod;

void main()
{
	out_colour = texture(u_TextureSampler, v_textureCoordinates) *  u_ColourMod;
}