#pragma once
#ifndef STARDUST_PHYSICS_H
#define STARDUST_PHYSICS_H

#include <type_traits>

#include <box2d/box2d.h>

namespace stardust
{
	namespace physics
	{
		enum BodyType
			: std::underlying_type_t<b2BodyType>
		{
			Static = b2_staticBody,
			Kinematic = b2_kinematicBody,
			Dynamic = b2_dynamicBody,
		};

		using BodyCreateInfo = b2BodyDef;
		using Body = b2Body;

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