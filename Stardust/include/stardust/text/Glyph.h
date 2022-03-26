#pragma once
#ifndef STARDUST_GLYPH_H
#define STARDUST_GLYPH_H

#include "stardust/graphics/texture/Texture.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

/*
 * Glyph metrics:
 * --------------
 *
 *                       xmin                     xmax
 *                        |                         |
 *                        |<-------- width -------->|
 *                        |                         |
 *              |         +-------------------------+----------------- ymax
 *              |         |    ggggggggg   ggggg    |     ^        ^
 *              |         |   g:::::::::ggg::::g    |     |        |
 *              |         |  g:::::::::::::::::g    |     |        |
 *              |         | g::::::ggggg::::::gg    |     |        |
 *              |         | g:::::g     g:::::g     |     |        |
 *   bearing_x -|-------->| g:::::g     g:::::g     |  bearing_y   |
 *              |         | g:::::g     g:::::g     |     |        |
 *              |         | g::::::g    g:::::g     |     |        |
 *              |         | g:::::::ggggg:::::g     |     |        |
 *              |         |  g::::::::::::::::g     |     |      height
 *              |         |   gg::::::::::::::g     |     |        |
 *  baseline ---*---------|---- gggggggg::::::g-----*--------      |
 *            / |         |             g:::::g     |              |
 *     origin   |         | gggggg      g:::::g     |              |
 *              |         | g:::::gg   gg:::::g     |              |
 *              |         |  g::::::ggg:::::::g     |              |
 *              |         |   gg:::::::::::::g      |              |
 *              |         |     ggg::::::ggg        |              |
 *              |         |         gggggg          |              v
 *              |         +-------------------------+----------------- ymin
 *              |                                   |
 *              |------------- advance_x ---------->|
 */

namespace stardust
{
    struct Glyph final
    {
        u32 codepoint;

        UVector2 size;
        IVector2 bearing;
        Vector2 advance;

        graphics::TextureCoordinatePair textureCoordinates;
    };

    struct ShapedGlyph final
    {
        u32 index;

        UVector2 size;
        IVector2 bearing;
        Vector2 advance;
        IVector2 offset;

        graphics::TextureCoordinatePair textureCoordinates;
    };
}

#endif
