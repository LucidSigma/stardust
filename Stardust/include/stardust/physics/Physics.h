#pragma once
#ifndef STARDUST_PHYSICS_H
#define STARDUST_PHYSICS_H

#include <box2d/box2d.h>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
	namespace physics
	{
		using Shape = b2Shape;
		using Chain = b2ChainShape;
		using Circle = b2CircleShape;
		using Edge = b2EdgeShape;

		using CollisionLayer = u16;

		constexpr CollisionLayer AllLayers = 0xFFFF;

		struct MassData
		{
			f32 mass;
			Vec2 centre;
			f32 momentOfInertia;
		};
	}

	namespace phys = physics;
}

#endif