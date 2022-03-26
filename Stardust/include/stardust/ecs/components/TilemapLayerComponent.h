#pragma once
#ifndef STARDUST_TILEMAP_LAYER_COMPONENT_H
#define STARDUST_TILEMAP_LAYER_COMPONENT_H

#include "stardust/tilemap/Tilemap.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct TilemapLayer final
        {
            ObserverPointer<Tilemap> tilemap = nullptr;
        };
    }
}

#endif
