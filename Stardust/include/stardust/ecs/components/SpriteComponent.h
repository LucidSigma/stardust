#pragma once
#ifndef STARDUST_SPRITE_COMPONENT_H
#define STARDUST_SPRITE_COMPONENT_H

#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/colour/Colours.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct Sprite final
        {
            ObserverPointer<const graphics::Texture> texture = nullptr;
            Optional<graphics::TextureCoordinatePair> subTextureArea = None;

            Colour colourMod = colours::White;
        };
    }
}

#endif
