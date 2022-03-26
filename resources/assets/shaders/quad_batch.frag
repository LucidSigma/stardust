#version 300 es

precision mediump float;

in vec4 v_colour;
in vec2 v_textureCoordinates;
in float v_textureIndex;

layout (location = 0) out vec4 out_colour;

uniform sampler2D u_Textures[16u];

vec4 getColourFromSamplerArray(uint index)
{
    // Sampler2D arrays can only be looked up with constant indices in GLSL ES 3.0.
    // Hence this workaround switch statement.
    switch (index)
    {
    default:
    case 0u:
        return texture(u_Textures[0u], v_textureCoordinates);

    case 1u:
        return texture(u_Textures[1u], v_textureCoordinates);

    case 2u:
        return texture(u_Textures[2u], v_textureCoordinates);

    case 3u:
        return texture(u_Textures[3u], v_textureCoordinates);

    case 4u:
        return texture(u_Textures[4u], v_textureCoordinates);

    case 5u:
        return texture(u_Textures[5u], v_textureCoordinates);

    case 6u:
        return texture(u_Textures[6u], v_textureCoordinates);

    case 7u:
        return texture(u_Textures[7u], v_textureCoordinates);

    case 8u:
        return texture(u_Textures[8u], v_textureCoordinates);

    case 9u:
        return texture(u_Textures[9u], v_textureCoordinates);

    case 10u:
        return texture(u_Textures[10u], v_textureCoordinates);

    case 11u:
        return texture(u_Textures[11u], v_textureCoordinates);

    case 12u:
        return texture(u_Textures[12u], v_textureCoordinates);

    case 13u:
        return texture(u_Textures[13u], v_textureCoordinates);

    case 14u:
        return texture(u_Textures[14u], v_textureCoordinates);

    case 15u:
        return texture(u_Textures[15u], v_textureCoordinates);
    }
}

void main()
{
    uint index = uint(v_textureIndex);

    out_colour = getColourFromSamplerArray(index) * v_colour;
}
