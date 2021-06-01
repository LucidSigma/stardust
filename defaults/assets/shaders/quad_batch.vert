#version 460 core

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec4 in_colour;
layout (location = 2) in vec2 in_textureCoordinates;
layout (location = 3) in float in_textureIndex;

layout (location = 0) out vec4 v_colour;
layout (location = 1) out vec2 v_textureCoordinates;
layout (location = 2) out float v_textureIndex;

uniform mat4 u_ViewProjection;

void main()
{
	v_colour = in_colour;
	v_textureCoordinates = in_textureCoordinates;
	v_textureIndex = in_textureIndex;	

	gl_Position = u_ViewProjection * vec4(in_position, 0.0, 1.0);
}