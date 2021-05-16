#pragma once
#ifndef STARDUST_ANIMATED_COMPONENT_H
#define STARDUST_ANIMATED_COMPONENT_H

#include "stardust/animation/Animator.h"
#include "stardust/data/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct Animated
        {
            ObserverPtr<const Animator> animator;

            Animated()
                : animator(nullptr)
            { }

            Animated(const Animator& animator)
                : animator(&animator)
            { }

            ~Animated() noexcept = default;
        };
    }
}

#endif