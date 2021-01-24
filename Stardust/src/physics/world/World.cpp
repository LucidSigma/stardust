#include "stardust/physics/world/World.h"

#include <utility>

namespace stardust
{
	namespace physics
	{
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

		void World::Raycast(RaycastCallback& callback, const Pair<Vec2, Vec2>& points) const
		{
			m_handle->RayCast(&callback, b2Vec2{ points.first.x, points.first.y }, b2Vec2{ points.second.x, points.second.y });
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