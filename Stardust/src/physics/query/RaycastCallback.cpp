#include "stardust/physics/query/RaycastCallback.h"

namespace stardust
{
	namespace physics
	{
		RaycastCallback::RaycastCallback(const RaycastFixtureReportCallback& fixtureReportCallback)
			: m_fixtureReportCallback(fixtureReportCallback)
		{ }

		f32 RaycastCallback::ReportFixture(Fixture* fixture, const Point& point, const Point& normal, const f32 fraction)
		{
			return m_fixtureReportCallback(*fixture, Vec2{ point.x, point.y }, Vec2{ normal.x, normal.y }, fraction);
		}
	}
}