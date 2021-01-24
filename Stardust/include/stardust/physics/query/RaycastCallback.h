#pragma once
#ifndef STARDUST_RAYCAST_CALLBACK_H
#define STARDUST_RAYCAST_CALLBACK_H

#include <functional>

#include <box2d/box2d.h>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace physics
	{
		using RaycastFixtureReportCallback = std::function<f32(Fixture&, Vec2, Vec2, f32)>;
		using ClipRaycast = f32;

		constexpr f32 IgnoreCurrentFixture = -1.0f;
		constexpr f32 TerminateRaycast = 0.0f;
		constexpr f32 ContinueRaycast = 1.0f;

		class RaycastCallback
			: public b2RayCastCallback
		{
		private:
			RaycastFixtureReportCallback m_fixtureReportCallback;

		public:
			explicit RaycastCallback(const RaycastFixtureReportCallback& fixtureReportCallback);
			virtual ~RaycastCallback() noexcept override = default;

			virtual f32 ReportFixture(Fixture* fixture, const Point& point, const Point& normal, const f32 fraction) override;
		};
	}
}

#endif