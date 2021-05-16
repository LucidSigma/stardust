#pragma once
#ifndef STARDUST_RIGID_BODY_COMPONENT
#define STARDUST_RIGID_BODY_COMPONENT

#include "stardust/data/Pointers.h"
#include "stardust/physics/body/Body.h"

namespace stardust
{
    namespace components
    {
        struct RigidBody
        {
            ObserverPtr<physics::Body> handle;

            RigidBody()
                : handle(nullptr)
            { }

            explicit RigidBody(physics::Body& handle)
                : handle(&handle)
            { }

            explicit RigidBody(ObserverPtr<physics::Body> handle)
                : handle(handle)
            { }

            ~RigidBody() noexcept = default;
        };
    }
}

#endif