#pragma once
#ifndef STARDUST_PHYSICS_BODY_COMPONENT
#define STARDUST_PHYSICS_BODY_COMPONENT

#include "stardust/data/Pointers.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace components
	{
		struct PhysicsBody
		{
			ObserverPtr<physics::Body> bodyHandle;

			PhysicsBody()
				: bodyHandle(nullptr)
			{ }

			explicit PhysicsBody(ObserverPtr<physics::Body> bodyHandle)
				: bodyHandle(bodyHandle)
			{ }

			~PhysicsBody() noexcept = default;
		};
	}
}

#endif