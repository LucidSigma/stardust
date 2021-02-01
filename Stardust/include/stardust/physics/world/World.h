#pragma once
#ifndef STARDUST_WORLD_H
#define STARDUST_WORLD_H

#include <functional>

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/body/Body.h"
#include "stardust/physics/collider/Collider.h"
#include "stardust/physics/raycast/RaycastHit.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
	namespace physics
	{
		using CollisionCallback = std::function<void(const Collider& other)>;

		class World
		{
		private:
			class CollisionListener
				: public b2ContactListener
			{
			private:
				const World& m_world;

			public:
				explicit CollisionListener(const World& world);
				virtual ~CollisionListener() noexcept = default;

				virtual void BeginContact(b2Contact* const contact) override;
				virtual void EndContact(b2Contact* const contact) override;
			};

			inline static u32 s_velocityIterations = 8u;
			inline static u32 s_positionIterations = 3u;

			UniquePtr<b2World> m_handle = nullptr;

			HashMap<const b2Body*, Body> m_bodies{ };

			CollisionListener m_collisionListener{ *this };
			HashMap<const b2Body*, CollisionCallback> m_collisionEnterCallbacks{ };
			HashMap<const b2Body*, CollisionCallback> m_collisionExitCallbacks{ };
			HashMap<const b2Body*, CollisionCallback> m_sensorEnterCallbacks{ };
			HashMap<const b2Body*, CollisionCallback> m_sensorExitCallbacks{ };

		public:
			inline static u32 GetVelocityIterations() noexcept { return s_velocityIterations; }
			inline static void SetVelocityIterations(const u32 iterations) noexcept { s_velocityIterations = iterations; }
			inline static u32 GetPositionIterations() noexcept { return s_positionIterations; }
			inline static void SetPositionIterations(const u32 iterations) noexcept { s_positionIterations = iterations; }

			World();
			explicit World(const Vec2& gravity);
			~World() noexcept = default;

			void Step(const f32 timestep) const;

			ObserverPtr<Body> CreateBody(const Body::CreateInfo& createInfo);
			void DestroyBody(ObserverPtr<const Body> body) noexcept;
			[[nodiscard]] ObserverPtr<Body> LookupBody(const ObserverPtr<const b2Body> bodyHandle);
			[[nodiscard]] ObserverPtr<const Body> LookupBody(const ObserverPtr<const b2Body> bodyHandle) const;

			inline void SetCollisionEnterCallback(const Body& body, const CollisionCallback& callback) { m_collisionEnterCallbacks[body.GetRawHandle()] = callback; }
			inline void SetCollisionExitCallback(const Body& body, const CollisionCallback& callback) { m_collisionExitCallbacks[body.GetRawHandle()] = callback; }
			inline void SetSensorEnterCallback(const Body& body, const CollisionCallback& callback) { m_sensorEnterCallbacks[body.GetRawHandle()] = callback; }
			inline void SetSensorExitCallback(const Body& body, const CollisionCallback& callback) { m_sensorExitCallbacks[body.GetRawHandle()] = callback; }

			[[nodiscard]] Optional<RaycastHit> Raycast(const Vec2& origin, const Vec2& direction, const f32 distance, const CollisionLayer layerMask = AllLayers) const;
			[[nodiscard]] Vector<RaycastHit> RaycastAll(const Vec2& origin, const Vec2& direction, const f32 distance, const CollisionLayer layerMask = AllLayers) const;

			[[nodiscard]] bool RaycastBox(const AABB& box, const CollisionLayer layerMask = AllLayers) const;
			[[nodiscard]] bool RaycastBox(const Vec2& centre, const Vec2& halfSize, const CollisionLayer layerMask = AllLayers) const;

			[[nodiscard]] Optional<Collider> QueryBox(const AABB& box, const CollisionLayer layerMask = AllLayers) const;
			[[nodiscard]] Optional<Collider> QueryBox(const Vec2& centre, const Vec2& halfSize, const CollisionLayer layerMask = AllLayers) const;
			[[nodiscard]] Vector<Collider> QueryBoxAll(const AABB& box, const CollisionLayer layerMask = AllLayers) const;
			[[nodiscard]] Vector<Collider> QueryBoxAll(const Vec2& centre, const Vec2& halfSize, const CollisionLayer layerMask = AllLayers) const;

			[[nodiscard]] Vec2 GetGravity() const;
			void SetGravity(const Vec2& gravity) const;

			[[nodiscard]] bool AllowsSleeping() const;
			void AllowSleeping(const bool allowSleeping) const;

			inline bool IsValid() const noexcept { return m_handle != nullptr; }
			[[nodiscard]] inline b2World* const GetRawHandle() const noexcept { return m_handle.get(); }
		};
	}
}

#endif