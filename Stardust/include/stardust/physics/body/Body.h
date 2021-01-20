#pragma once
#ifndef STARDUST_BODY_H
#define STARDUST_BODY_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <box2d/box2d.h>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

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

				bool fixedRotation = false;
				bool isBullet = false;

				bool isEnabled = true;

				f32 gravityScale = 1.0f;
			};

		private:
			ObserverPtr<b2Body> m_handle = nullptr;
			ObserverPtr<const class World> m_owningWorld = nullptr;

		public:
			Body(const class World& world, const CreateInfo& createInfo);
			Body(Body&& other) noexcept;
			Body& operator =(Body&& other) noexcept;
			~Body() noexcept = default;

			[[nodiscard]] Vec2 GetWorldCentre() const;
			[[nodiscard]] Vec2 GetLocalCenter() const;

			[[nodiscard]] Vec2 GetPosition() const;
			void SetPosition(const Vec2& position) const;
			f32 GetRotation() const;
			void SetRotation(const f32 rotation) const;

			[[nodiscard]] Vec2 GetLinearVelocity() const;
			void SetLinearVelocity(const Vec2& linearVelocity) const;
			f32 GetAngularVelocity() const;
			void SetAngularVelocity(const f32 angularVelocity) const;

			ObserverPtr<b2Body> GetRawHandle() const noexcept { return m_handle; }
			ObserverPtr<const class World> GetOwningWorld() const noexcept { return m_owningWorld; }
		};
	}
}

#endif