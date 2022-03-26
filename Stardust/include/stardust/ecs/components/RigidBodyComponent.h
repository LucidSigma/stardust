#pragma once
#ifndef STARDUST_RIGID_BODY_COMPONENT_H
#define STARDUST_RIGID_BODY_COMPONENT_H

#include "stardust/physics/body/Body.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct RigidBody final
        {
            ObserverPointer<physics::Body> handle = nullptr;
        };
    }
}

#endif
