#include "stardust/physics/body/Body.h"

#include <utility>


#include "stardust/physics/world/World.h"

namespace stardust
{
	namespace physics
	{
		Body::Body(const World& world, const Body::CreateInfo& createInfo)
		{
			b2BodyDef bodyDef{ };
			bodyDef.type = static_cast<b2BodyType>(createInfo.type);
			bodyDef.position = b2Vec2{ createInfo.position.x, createInfo.position.y };
			bodyDef.angle = createInfo.angle;
			bodyDef.linearVelocity = b2Vec2{ createInfo.linearVelocity.x, createInfo.linearVelocity.y };
			bodyDef.angularVelocity = createInfo.angularVelocity;
			bodyDef.linearDamping = createInfo.linearDamping;
			bodyDef.angularDamping = createInfo.angularDamping;
			bodyDef.allowSleep = createInfo.allowSleep;
			bodyDef.awake = createInfo.isAwake;
			bodyDef.fixedRotation = createInfo.fixedRotation;
			bodyDef.bullet = createInfo.isBullet;
			bodyDef.enabled = createInfo.isEnabled;
			bodyDef.gravityScale = createInfo.gravityScale;

			m_handle = world.GetRawHandle()->CreateBody(&bodyDef);
			m_owningWorld = &world;
		}

		Body::Body(Body&& other) noexcept
			: m_handle(nullptr), m_owningWorld(nullptr)
		{
			std::swap(m_handle, other.m_handle);
			std::swap(m_owningWorld, other.m_owningWorld);
		}

		Body& Body::operator =(Body&& other) noexcept
		{
			m_handle = std::exchange(other.m_handle, nullptr);
			m_owningWorld = std::exchange(other.m_owningWorld, nullptr);

			return *this;
		}
	}
}