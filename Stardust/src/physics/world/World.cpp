#include "stardust/physics/world/World.h"

#include <utility>

#include "stardust/math/Math.h"

namespace stardust
{
	namespace physics
	{
		namespace
		{
			class RaycastCallback
				: public b2RayCastCallback
			{
			private:
				const Vec2& m_origin;
				CollisionLayer m_layerMask = 0u;

				bool& m_hasHitFixture;
				RaycastHit& m_raycastHitData;

			public:
				inline RaycastCallback(const Vec2& origin, const CollisionLayer layerMask, bool& hasHitFixture, RaycastHit& raycastHitData)
					: m_origin(origin), m_layerMask(layerMask), m_hasHitFixture(hasHitFixture), m_raycastHitData(raycastHitData)
				{ }

				virtual ~RaycastCallback() noexcept override = default;

				inline virtual f32 ReportFixture(Fixture* const fixture, const Point& point, const Point& normal, const f32 fraction) override
				{
					if (fixture->GetFilterData().maskBits & m_layerMask)
					{
						m_hasHitFixture = true;

						m_raycastHitData.fraction = fraction;
						m_raycastHitData.fixture = fixture;
						m_raycastHitData.normal = Vec2{ normal.x, normal.y };
						m_raycastHitData.point = Vec2{ point.x, point.y };

						m_raycastHitData.distance = glm::distance(m_origin, m_raycastHitData.point);

						return fraction;
					}
					else
					{
						return -1.0f;
					}
				}
			};
		}

		World::World()
			: m_handle(std::make_unique<b2World>(b2Vec2_zero))
		{ }

		World::World(const Vec2& gravity)
			: m_handle(std::make_unique<b2World>(b2Vec2{ gravity.x, gravity.y }))
		{ }

		void World::Step(const f32 timestep) const
		{
			m_handle->Step(timestep, static_cast<i32>(s_velocityIterations), static_cast<i32>(s_positionIterations));
		}

		ObserverPtr<Body> World::CreateBody(const Body::CreateInfo& createInfo)
		{
			Body body(*this, createInfo);
			const b2Body* rawBodyHandle = body.GetRawHandle();
			m_bodies[rawBodyHandle] = std::move(body);

			return &m_bodies[rawBodyHandle];
		}

		void World::DestroyBody(ObserverPtr<const Body> body) noexcept
		{
			m_bodies.erase(body->GetRawHandle());
			m_handle->DestroyBody(body->GetRawHandle());
		}

		[[nodiscard]] Optional<RaycastHit> World::Raycast(const Vec2& origin, const Vec2& direction, const f32 distance, const CollisionLayer layerMask) const
		{
			const Vec2 destinationPoint = origin + direction * distance;

			if (origin == destinationPoint)
			{
				return NullOpt;
			}

			bool hasHitAnything = false;
			RaycastHit raycastHitData{ };

			RaycastCallback callback(origin, layerMask, hasHitAnything, raycastHitData);
			m_handle->RayCast(&callback, b2Vec2{ origin.x, origin.y }, b2Vec2{ destinationPoint.x, destinationPoint.y });

			if (hasHitAnything)
			{
				return raycastHitData;
			}
			else
			{
				return NullOpt;
			}
		}

		void World::QueryAABB(AABBCallback& callback, const AABB& aabb) const
		{
			m_handle->QueryAABB(&callback, aabb);
		}

		[[nodiscard]] Vec2 World::GetGravity() const
		{
			const b2Vec2 gravity = m_handle->GetGravity();

			return Vec2{ gravity.x, gravity.y };
		}

		void World::SetGravity(const Vec2& gravity) const
		{
			m_handle->SetGravity(b2Vec2{ gravity.x, gravity.y });
		}

		bool World::AllowsSleeping() const
		{
			return m_handle->GetAllowSleeping();
		}

		void World::AllowSleeping(const bool allowSleeping) const
		{
			return m_handle->SetAllowSleeping(allowSleeping);
		}
	}
}