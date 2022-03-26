#pragma once
#ifndef STARDUST_RENDERABLE_COMPONENT_H
#define STARDUST_RENDERABLE_COMPONENT_H

#include "stardust/graphics/sorting_layer/SortingLayer.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct Renderable final
        {
            graphics::SortingLayer sortingLayer{ 0.0f };
            i32 orderInLayer = 0;
        };
    }
}

#endif
