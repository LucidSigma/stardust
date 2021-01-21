#pragma once
#ifndef STARDUST_BODY_H
#define STARDUST_BODY_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace physics
	{
		class Body
			: private INoncopyable
		{
		public:
			enum Type
				: std::underlying_type_t<b2BodyType>
			{
				Static = b2_staticBody,
				Kinematic = b2_kinematicBody,
				Dynamic = b2_dynamicBody,
			};

			struct CreateInfo
			{
				Type type = Type::Static;

				Vec2 position{ 0.0f, 0.0f };
				f32 angle = 0.0f;

				Vec2 linearVelocity{ 0.0f, 0.0f };
				f32 angularVelocity = 0.0f;

				f32 linearDamping = 0.0f;
				f32 angularDamping = 0.0f;

				bool allowSleep = true;
				bool isAwake = true;

				bool hasFixedRotation = false;
				bool isBullet = false;

				bool isEnabled = true;

				f32 gravityScale = 1.0f;
			};

			struct MassData
			{
				f32 mass;
				Vec2 centre;
				f32 momentOfInertia;
			};

		private:
			ObserverPtr<b2Body> m_handle = nullptr;
			ObserverPtr<const class World> m_owningWorld = nullptr;

			HashSet<ObserverPtr<Fixture>> m_fixtures{ };

		public:
			Body() = default;
			Body(const class World& world, const CreateInfo& createInfo);
			Body(Body&& other) noexcept;
			Body& operator =(Body&& other) noexcept;
			~Body() noexcept = default;

			void Initialise(const class World& world, const CreateInfo& createInfo);

			void ApplyForce(const Vec2& force, const Vec2& point, const bool wakeUp) const;
			void ApplyForceToCentre(const Vec2& force, const bool wakeUp) const;
			void ApplyTorque(const f32 torque, const bool wakeUp) const;

			void ApplyLinearImpulse(const Vec2& impulse, const Vec2& point, const bool wakeUp) const;
			void ApplyLinearImpulseToCentre(const Vec2& impulse, const bool wakeUp) const;
			void ApplyAngularImpulse(const f32 impulse, const bool wakeUp) const;

			ObserverPtr<Fixture> AddFixture(const FixtureInfo& fixtureInfo);
			void RemoveFixture(ObserverPtr<Fixture> fixture);
			inline const HashSet<ObserverPtr<Fixture>>& GetFixtures() const { return m_fixtures; }

			[[nodiscard]] Vec2 GetWorldCentre() const;
			[[nodiscard]] Vec2 GetLocalCenter() const;

			[[nodiscard]] Vec2 GetWorldPoint(const Vec2& localPoint) const;
			[[nodiscard]] Vec2 GetWorldVector(const Vec2& localVector) const;
			[[nodiscard]] Vec2 GetLocalPoint(const Vec2& worldPoint) const;
			[[nodiscard]] Vec2 GetLocalVector(const Vec2& worldVector) const;
			[[nodiscard]] Vec2 GetLinearVelocityFromWorldPoint(const Vec2& worldPoint) const;
			[[nodiscard]] Vec2 GetLinearVelocityFromLocalPoint(const Vec2& localPoint) const;

			bool IsEnabled() const;
			void SetEnabled(const bool isEnabled) const;
			bool IsAwake() const;
			bool CanSleep() const;
			void SetAllowSleeping(const bool canSleep) const;

			Type GetType() const;
			void SetType(const Type type) const;

			bool IsBullet() const;
			void SetBullet(const bool isBullet) const;

			[[nodiscard]] Vec2 GetPosition() const;
			void SetPosition(const Vec2& position) const;
			[[nodiscard]] f32 GetRotation() const;
			void SetRotation(const f32 rotation) const;

			bool HasFixedRotation() const;
			void SetFixedRotation(const bool hasFixedRotation) const;

			[[nodiscard]] Vec2 GetLinearVelocity() const;
			void SetLinearVelocity(const Vec2& linearVelocity) const;
			f32 GetAngularVelocity() const;
			void SetAngularVelocity(const f32 angularVelocity) const;

			f32 GetLinearDamping() const;
			void SetLinearDamping(const f32 linearDamping) const;
			f32 GetAngularDamping() const;
			void SetAngularDamping(const f32 angularDamping) const;

			f32 GetGravityScale() const;
			void SetGravityScale(const f32 gravityScale) const;

			f32 GetMass() const;
			f32 GetInertia() const;
			[[nodiscard]] MassData GetMassData() const;
			void SetMassData(const MassData& massData) const;
			void ResetMassData() const;

			ObserverPtr<b2Body> GetRawHandle() const noexcept { return m_handle; }
			ObserverPtr<const class World> GetOwningWorld() const noexcept { return m_owningWorld; }
		};
	}
}

#endif