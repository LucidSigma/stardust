#include "stardust/physics/world/World.h"

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

		[[nodiscard]] Body World::CreateBody(const Body::CreateInfo& createInfo) const
		{
			return Body(*this, createInfo);
		}

		void World::DestroyBody(const Body& body) const noexcept
		{
			m_handle->DestroyBody(body.GetRawHandle());
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