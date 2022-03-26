#pragma once
#ifndef STARDUST_ANIMATED_COMPONENT_H
#define STARDUST_ANIMATED_COMPONENT_H

#include "stardust/animation/Animator.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct Animated final
        {
            ObserverPointer<const animation::Animator> animator = nullptr;
        };
    }
}

#endif
