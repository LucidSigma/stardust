#pragma once
#ifndef STARDUST_TILEMAP_LAYER_COMPONENT_H
#define STARDUST_TILEMAP_LAYER_COMPONENT_H

#include "stardust/data/Pointers.h"
#include "stardust/tilemap/Tilemap.h"

namespace stardust
{
    namespace components
    {
        struct TilemapLayer
        {
            ObserverPtr<Tilemap::Layer> layer;

            TilemapLayer()
                : layer(nullptr)
            { }

            explicit TilemapLayer(Tilemap::Layer& layer)
                : layer(&layer)
            { }

            ~TilemapLayer() noexcept = default;
        };
    }
}

#endif