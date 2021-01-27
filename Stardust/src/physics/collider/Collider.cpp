#include "stardust/physics/collider/Collider.h"

#include "stardust/physics/body/Body.h"
#include "stardust/physics/world/World.h"

namespace stardust
{
	namespace physics
	{
		Collider::Collider(const Body& body, const CreateInfo& createInfo)
			: m_owningBody(&body)
		{
			b2FixtureDef fixtureDef{ };
			fixtureDef.shape = createInfo.shape;
			fixtureDef.friction = createInfo.friction;
			fixtureDef.restitution = createInfo.restitution;
			fixtureDef.restitutionThreshold = createInfo.restitutionThreshold;
			fixtureDef.density = createInfo.density;
			fixtureDef.isSensor = createInfo.isSensor;
			fixtureDef.userData = b2FixtureUserData{ };

			body.GetRawHandle()->CreateFixture(&fixtureDef);
		}

		Collider::Collider(const ObserverPtr<b2Fixture> fixtureHandle, const World& world)
			: m_handle(fixtureHandle), m_owningBody(world.LookupBody(fixtureHandle->GetBody()))
		{ }
	}
}