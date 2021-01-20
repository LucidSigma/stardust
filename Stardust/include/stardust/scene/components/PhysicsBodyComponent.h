#pragma once
#ifndef STARDUST_PHYSICS_BODY_COMPONENT
#define STARDUST_PHYSICS_BODY_COMPONENT

#include "stardust/data/Pointers.h"
#include "stardust/physics/body/Body.h"

namespace stardust
{
	namespace components
	{
		struct PhysicsBody
		{
			ObserverPtr<physics::Body> handle;

			PhysicsBody()
				: handle(nullptr)
			{ }

			explicit PhysicsBody(physics::Body& handle)
				: handle(&handle)
			{ }

			~PhysicsBody() noexcept = default;
		};
	}
}

#endif