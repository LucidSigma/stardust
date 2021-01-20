#pragma once
#ifndef STARDUST_PHYSICS_BODY_COMPONENT
#define STARDUST_PHYSICS_BODY_COMPONENT

#include <utility>

#include "stardust/physics/body/Body.h"

namespace stardust
{
	namespace components
	{
		struct PhysicsBody
		{
			physics::Body bodyHandle;

			explicit PhysicsBody(physics::Body&& bodyHandle)
				: bodyHandle(std::move(bodyHandle))
			{ }

			~PhysicsBody() noexcept = default;
		};
	}
}

#endif