#pragma once
#ifndef STARDUST_AABB_CALLBACK_H
#define STARDUST_AABB_CALLBACK_H

#include <functional>

#include <box2d/box2d.h>

#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace physics
	{
		using AABBFixtureReportCallback = std::function<bool(Fixture&)>;

		class AABBCallback
			: public b2QueryCallback
		{
		private:
			AABBFixtureReportCallback m_fixtureReportCallback;

		public:
			AABBCallback(const AABBFixtureReportCallback& fixtureReportCallback);
			virtual ~AABBCallback() noexcept override = default;

			virtual bool ReportFixture(Fixture* fixture) override;
		};
	}
}

#endif