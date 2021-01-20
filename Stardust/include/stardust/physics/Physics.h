#pragma once
#ifndef STARDUST_PHYSICS_H
#define STARDUST_PHYSICS_H

#include <box2d/box2d.h>

namespace stardust
{
	namespace physics
	{
		using Fixture = b2Fixture;
		using FixtureInfo = b2FixtureDef;

		using Point = b2Vec2;
		using Chain = b2ChainShape;
		using Circle = b2CircleShape;
		using Edge = b2EdgeShape;
		using Polygon = b2PolygonShape;
	}

	namespace phys = physics;
}

#endif