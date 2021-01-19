#pragma once
#ifndef STARDUST_WORLD_H
#define STARDUST_WORLD_H

#include <box2d/box2d.h>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	namespace physics
	{
		class World
		{
		private:
			inline static u32 s_velocityIterations = 8u;
			inline static u32 s_positionIterations = 3u;

			UniquePtr<b2World> m_handle = nullptr;

		public:
			inline static u32 GetVelocityIterations() noexcept { return s_velocityIterations; }
			inline static void SetVelocityIterations(const u32 iterations) noexcept { s_velocityIterations = iterations; }
			inline static u32 GetPositionIterations() noexcept { return s_positionIterations; }
			inline static void SetPositionIterations(const u32 iterations) noexcept { s_positionIterations = iterations; }

			World();
			World(const Vec2& gravity);
			~World() noexcept = default;

			void Step(const f32 timestep) const;

			[[nodiscard]] Vec2 GetGravity() const;
			void SetGravity(const Vec2& gravity) const;

			inline bool IsValid() const noexcept { return m_handle != nullptr; }
			[[nodiscard]] inline b2World* const GetRawHandle() const noexcept { return m_handle.get(); }
		};
	}
}

#endif