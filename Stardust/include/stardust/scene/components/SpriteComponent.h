#pragma once
#ifndef STARDUST_SPRITE_COMPONENT_H
#define STARDUST_SPRITE_COMPONENT_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"

namespace stardust
{
    namespace components
    {
        struct Sprite
        {
            ObserverPtr<const Texture> texture;
            Optional<TextureCoordinatePair> subTextureArea;
            Colour colourMod;

            Sprite()
                : texture(nullptr), subTextureArea(NullOpt), colourMod(colours::White)
            { }

            Sprite(const Texture& texture, const Optional<TextureCoordinatePair>& subtextureArea = NullOpt, const Colour colourMod = colours::White)
                : texture(&texture), subTextureArea(subtextureArea), colourMod(colourMod)
            { }

            Sprite(ObserverPtr<const Texture> texture, const Optional<TextureCoordinatePair>& subtextureArea = NullOpt, const Colour colourMod = colours::White)
                : texture(texture), subTextureArea(subtextureArea), colourMod(colourMod)
            { }

            ~Sprite() noexcept = default;
        };
    }
}

#endif