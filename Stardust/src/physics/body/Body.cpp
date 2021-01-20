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
			bodyDef.fixedRotation = createInfo.hasFixedRotation;
			bodyDef.bullet = createInfo.isBullet;
			bodyDef.enabled = createInfo.isEnabled;
			bodyDef.gravityScale = createInfo.gravityScale;

			m_handle = world.GetRawHandle()->CreateBody(&bodyDef);
			m_owningWorld = &world;
		}

		Body::Body(Body&& other) noexcept
			: m_handle(nullptr), m_owningWorld(nullptr), m_fixtures({ })
		{
			std::swap(m_handle, other.m_handle);
			std::swap(m_owningWorld, other.m_owningWorld);
			std::swap(m_fixtures, other.m_fixtures);
		}

		Body& Body::operator =(Body&& other) noexcept
		{
			m_handle = std::exchange(other.m_handle, nullptr);
			m_owningWorld = std::exchange(other.m_owningWorld, nullptr);
			m_fixtures = std::exchange(other.m_fixtures, { });

			return *this;
		}

		void Body::ApplyForce(const Vec2& force, const Vec2& point, const bool wakeUp) const
		{
			m_handle->ApplyForce(b2Vec2{ force.x, force.y }, b2Vec2{ point.x, point.y }, wakeUp);
		}

		void Body::ApplyForceToCentre(const Vec2& force, const bool wakeUp) const
		{
			m_handle->ApplyForceToCenter(b2Vec2{ force.x, force.y }, wakeUp);
		}

		void Body::ApplyTorque(const f32 torque, const bool wakeUp) const
		{
			m_handle->ApplyTorque(torque, wakeUp);
		}

		void Body::ApplyLinearImpulse(const Vec2& impulse, const Vec2& point, const bool wakeUp) const
		{
			m_handle->ApplyLinearImpulse(b2Vec2{ impulse.x, impulse.y }, b2Vec2{ point.x, point.y }, wakeUp);
		}

		void Body::ApplyLinearImpulseToCentre(const Vec2& impulse, const bool wakeUp) const
		{
			m_handle->ApplyLinearImpulseToCenter(b2Vec2{ impulse.x, impulse.y }, wakeUp);
		}

		void Body::ApplyAngularImpulse(const f32 impulse, const bool wakeUp) const
		{
			m_handle->ApplyAngularImpulse(impulse, wakeUp);
		}

		ObserverPtr<Fixture> Body::AddFixture(const FixtureInfo& fixtureInfo)
		{
			ObserverPtr<Fixture> fixture = m_handle->CreateFixture(&fixtureInfo);
			m_fixtures.insert(fixture);

			return fixture;
		}

		void Body::RemoveFixture(ObserverPtr<Fixture> fixture)
		{
			m_fixtures.erase(fixture);
			m_handle->DestroyFixture(fixture);
		}

		[[nodiscard]] Vec2 Body::GetWorldCentre() const
		{
			const b2Vec2& worldCentre = m_handle->GetWorldCenter();

			return Vec2{ worldCentre.x, worldCentre.y };
		}

		[[nodiscard]] Vec2 Body::GetLocalCenter() const
		{
			const b2Vec2& localCentre = m_handle->GetLocalCenter();

			return Vec2{ localCentre.x, localCentre.y };
		}

		[[nodiscard]] Vec2 Body::GetWorldPoint(const Vec2& localPoint) const
		{
			const b2Vec2 worldPoint = m_handle->GetWorldPoint(b2Vec2{ localPoint.x, localPoint.y });
			
			return Vec2{ worldPoint.x, worldPoint.y };
		}

		[[nodiscard]] Vec2 Body::GetWorldVector(const Vec2& localVector) const
		{
			const b2Vec2 worldVector = m_handle->GetWorldVector(b2Vec2{ localVector.x, localVector.y });

			return Vec2{ worldVector.x, worldVector.y };
		}

		[[nodiscard]] Vec2 Body::GetLocalPoint(const Vec2& worldPoint) const
		{
			const b2Vec2 localPoint = m_handle->GetLocalPoint(b2Vec2{ worldPoint.x, worldPoint.y });

			return Vec2{ localPoint.x, localPoint.y };
		}

		[[nodiscard]] Vec2 Body::GetLocalVector(const Vec2& worldVector) const
		{
			const b2Vec2 localVector = m_handle->GetLocalVector(b2Vec2{ worldVector.x, worldVector.y });

			return Vec2{ localVector.x, localVector.y };
		}

		[[nodiscard]] Vec2 Body::GetLinearVelocityFromWorldPoint(const Vec2& worldPoint) const
		{
			const b2Vec2 linearVelocity = m_handle->GetLinearVelocityFromWorldPoint(b2Vec2{ worldPoint.x, worldPoint.y });

			return Vec2{ linearVelocity.x, linearVelocity.y };
		}

		[[nodiscard]] Vec2 Body::GetLinearVelocityFromLocalPoint(const Vec2& localPoint) const
		{
			const b2Vec2 linearVelocity = m_handle->GetLinearVelocityFromLocalPoint(b2Vec2{ localPoint.x, localPoint.y });

			return Vec2{ linearVelocity.x, linearVelocity.y };
		}

		bool Body::IsEnabled() const
		{
			return m_handle->IsEnabled();
		}

		void Body::SetEnabled(const bool isEnabled) const
		{
			m_handle->SetEnabled(isEnabled);
		}

		bool Body::IsAwake() const
		{
			return m_handle->IsAwake();
		}

		bool Body::CanSleep() const
		{
			return m_handle->IsSleepingAllowed();
		}

		void Body::SetAllowSleeping(const bool canSleep) const
		{
			m_handle->SetSleepingAllowed(canSleep);
		}

		Body::Type Body::GetType() const
		{
			return static_cast<Type>(m_handle->GetType());
		}

		void Body::SetType(const Type type) const
		{
			m_handle->SetType(static_cast<b2BodyType>(type));
		}

		bool Body::IsBullet() const
		{
			return m_handle->IsBullet();
		}

		void Body::SetBullet(const bool isBullet) const
		{
			m_handle->SetBullet(isBullet);
		}

		[[nodiscard]] Vec2 Body::GetPosition() const
		{
			const b2Vec2& position = m_handle->GetPosition();

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

		bool Body::HasFixedRotation() const
		{
			return m_handle->IsFixedRotation();
		}

		void Body::SetFixedRotation(const bool hasFixedRotation) const
		{
			m_handle->SetFixedRotation(hasFixedRotation);
		}

		[[nodiscard]] Vec2 Body::GetLinearVelocity() const
		{
			const b2Vec2& linearVelocity = m_handle->GetLinearVelocity();

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

		f32 Body::GetLinearDamping() const
		{
			return m_handle->GetLinearDamping();
		}

		void Body::SetLinearDamping(const f32 linearDamping) const
		{
			m_handle->SetLinearDamping(linearDamping);
		}

		f32 Body::GetAngularDamping() const
		{
			return m_handle->GetAngularDamping();
		}

		void Body::SetAngularDamping(const f32 angularDamping) const
		{
			m_handle->SetAngularDamping(angularDamping);
		}

		f32 Body::GetGravityScale() const
		{
			return m_handle->GetGravityScale();
		}

		void Body::SetGravityScale(const f32 gravityScale) const
		{
			m_handle->SetGravityScale(gravityScale);
		}
		
		f32 Body::GetMass() const
		{
			return m_handle->GetMass();
		}

		f32 Body::GetInertia() const
		{
			return m_handle->GetInertia();
		}

		[[nodiscard]] Body::MassData Body::GetMassData() const
		{
			b2MassData massData{ };
			m_handle->GetMassData(&massData);

			return MassData{
				.mass = massData.mass,
				.centre = Vec2{ massData.center.x, massData.center.y },
				.momentOfInertia = massData.I,
			};
		}

		void Body::SetMassData(const MassData& massData) const
		{
			const b2MassData convertedMassData{
				.mass = massData.mass,
				.center = b2Vec2{ massData.centre.x, massData.centre.y },
				.I = massData.momentOfInertia,
			};

			m_handle->SetMassData(&convertedMassData);
		}

		void Body::ResetMassData() const
		{
			m_handle->ResetMassData();
		}
	}
}