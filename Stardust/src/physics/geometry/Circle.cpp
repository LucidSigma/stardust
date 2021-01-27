#include "stardust/physics/geometry/Circle.h"

namespace stardust
{
	namespace physics
	{
		Circle::Circle(const b2CircleShape& shapeHandle)
			: m_circle(shapeHandle)
		{ }

		Circle::Circle(const f32 radius, const Vec2& position)
		{
			m_circle.m_radius = radius;
			m_circle.m_p = b2Vec2{ position.x, position.y };
		}
		
		[[nodiscard]] Vec2 Circle::GetPosition() const noexcept
		{
			return Vec2{ m_circle.m_p.x, m_circle.m_p.y };
		}

		void Circle::SetPosition(const Vec2& position) noexcept
		{
			m_circle.m_p = b2Vec2{ position.x, position.y };
		}

		[[nodiscard]] bool Circle::TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			return m_circle.TestPoint(transform, b2Vec2{ point.x, point.y });
		}

		[[nodiscard]] AABB Circle::ComputeAABB(const Vec2& worldPosition, const f32 rotation) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			b2AABB aabb{ };
			m_circle.ComputeAABB(&aabb, transform, 0);

			return aabb;
		}

		[[nodiscard]] MassData Circle::ComputeMassData(const f32 density) const
		{
			b2MassData massData{ };
			m_circle.ComputeMass(&massData, density);

			return MassData{
				.mass = massData.mass,
				.centre = Vec2{ massData.center.x, massData.center.y },
				.momentOfInertia = massData.I,
			};
		}
	}
}