#include "stardust/physics/query/AABBCallback.h"

namespace stardust
{
	namespace physics
	{
		AABBCallback::AABBCallback(AABBFixtureReportCallback& fixtureReportCallback)
			: m_fixtureReportCallback(fixtureReportCallback)
		{ }

		bool AABBCallback::ReportFixture(Fixture* fixture)
		{
			return m_fixtureReportCallback(*fixture);
		}
	}
}