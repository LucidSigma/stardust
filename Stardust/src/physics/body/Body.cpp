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

		[[nodiscard]] Vec2 Body::GetWorldCentre() const
		{
			const auto& worldCentre = m_handle->GetWorldCenter();

			return Vec2{ worldCentre.x, worldCentre.y };
		}

		[[nodiscard]] Vec2 Body::GetLocalCenter() const
		{
			const auto& localCentre = m_handle->GetLocalCenter();

			return Vec2{ localCentre.x, localCentre.y };
		}

		[[nodiscard]] Vec2 Body::GetPosition() const
		{
			const auto& position = m_handle->GetPosition();

			return Vec2{ position.x, position.y };
		}

		void Body::SetPosition(const Vec2& position) const
		{
			m_handle->SetTransform(b2Vec2{ position.x, position.y }, GetRotation());
		}

		f32 Body::GetRotation() const
		{
			return m_handle->GetAngle();
		}

		void Body::SetRotation(const f32 rotation) const
		{
			m_handle->SetTransform(m_handle->GetPosition(), rotation);
		}

		[[nodiscard]] Vec2 Body::GetLinearVelocity() const
		{
			const auto& linearVelocity = m_handle->GetLinearVelocity();

			return Vec2{ linearVelocity.x, linearVelocity.y };
		}

		void Body::SetLinearVelocity(const Vec2& linearVelocity) const
		{
			m_handle->SetLinearVelocity(b2Vec2{ linearVelocity.x, linearVelocity.y });
		}

		f32 Body::GetAngularVelocity() const
		{
			return m_handle->GetAngularVelocity();
		}

		void Body::SetAngularVelocity(const f32 angularVelocity) const
		{
			m_handle->SetAngularVelocity(angularVelocity);
		}
	}
}