#pragma once
#ifndef STARDUST_RENDERABLE_COMPONENT_H
#define STARDUST_RENDERABLE_COMPONENT_H

#include "stardust/data/Pointers.h"
#include "stardust/graphics/SortingLayer.h"

namespace stardust
{
    namespace components
    {
        struct Renderable
        {
            ObserverPtr<const SortingLayer> sortingLayer;

            Renderable()
                : sortingLayer(nullptr)
            { }

            explicit Renderable(const SortingLayer& sortingLayer )
                : sortingLayer(&sortingLayer)
            { }

            ~Renderable() noexcept = default;
        };
    }
}

#endif