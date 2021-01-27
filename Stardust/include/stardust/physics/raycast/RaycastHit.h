#pragma once
#ifndef STARDUST_RAYCAST_H
#define STARDUST_RAYCAST_H

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace physics
	{
		struct RaycastHit
		{
			Collider collider;

			f32 distance;
			f32 fraction;

			Vec2 normal;
			Vec2 point;
		};
	}
}

#endif