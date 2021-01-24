#include "stardust/physics/query/RaycastCallback.h"

namespace stardust
{
	namespace physics
	{
		OldRaycastCallback::OldRaycastCallback(const RaycastFixtureReportCallback& fixtureReportCallback)
			: m_fixtureReportCallback(fixtureReportCallback)
		{ }

		f32 OldRaycastCallback::ReportFixture(Fixture* fixture, const Point& point, const Point& normal, const f32 fraction)
		{
			return m_fixtureReportCallback(*fixture, Vec2{ point.x, point.y }, Vec2{ normal.x, normal.y }, fraction);
		}
	}
}