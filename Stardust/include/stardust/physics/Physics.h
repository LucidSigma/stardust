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
		using CollisionLayer = u16;

		constexpr CollisionLayer AllLayers = 0xFFFF;

		enum class ShapeType
		{
			Circle = b2Shape::e_circle,
			Edge = b2Shape::e_edge,
			Polygon = b2Shape::e_polygon,
			Chain = b2Shape::e_chain,
		};

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