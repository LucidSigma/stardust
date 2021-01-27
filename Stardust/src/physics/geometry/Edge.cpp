#include "stardust/physics/geometry/Edge.h"

#include "stardust/math/Math.h"

namespace stardust
{
	namespace physics
	{
		Edge::Edge(const Vec2& vertexA, const Vec2& vertexB)
		{
			SetTwoSided(vertexA, vertexB);
		}

		Edge::Edge(const Vec2& ghostVertexA, const Vec2& vertexA, const Vec2& vertexB, const Vec2& ghostVertexB)
		{
			SetOneSided(ghostVertexA, vertexA, vertexB, ghostVertexB);
		}

		void Edge::SetOneSided(const Vec2& ghostVertexA, const Vec2& vertexA, const Vec2& vertexB, const Vec2& ghostVertexB)
		{
			m_edge.SetOneSided(
				b2Vec2{ ghostVertexA.x, ghostVertexA.y },
				b2Vec2{ vertexA.x, vertexA.y },
				b2Vec2{ vertexB.x, vertexB.y },
				b2Vec2{ ghostVertexB.x, ghostVertexB.y }
			);
		}

		void Edge::SetTwoSided(const Vec2& vertexA, const Vec2& vertexB)
		{
			m_edge.SetTwoSided(
				b2Vec2{ vertexA.x, vertexA.y },
				b2Vec2{ vertexB.x, vertexB.y }
			);
		}

		[[nodiscard]] Pair<Vec2, Vec2> Edge::GetVertices() const
		{
			const Vec2 vertexA{ m_edge.m_vertex1.x, m_edge.m_vertex1.y };
			const Vec2 vertexB{ m_edge.m_vertex2.x, m_edge.m_vertex2.y };

			return  Pair<Vec2, Vec2>{ vertexA, vertexB };
		}

		[[nodiscard]] Pair<Vec2, Vec2> Edge::GetGhostVertices() const
		{
			const Vec2 ghostVertexA{ m_edge.m_vertex0.x, m_edge.m_vertex0.y };
			const Vec2 ghostVertexB{ m_edge.m_vertex3.x, m_edge.m_vertex3.y };

			return  Pair<Vec2, Vec2>{ ghostVertexA, ghostVertexB };
		}

		[[nodiscard]] bool Edge::TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			return m_edge.TestPoint(transform, b2Vec2{ point.x, point.y });
		}

		[[nodiscard]] AABB Edge::ComputeAABB(const Vec2& worldPosition, const f32 rotation) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			b2AABB aabb{ };
			m_edge.ComputeAABB(&aabb, transform, 0);

			return aabb;
		}

		[[nodiscard]] MassData Edge::ComputeMassData(const f32 density) const
		{
			b2MassData massData{ };
			m_edge.ComputeMass(&massData, density);

			return MassData{
				.mass = massData.mass,
				.centre = Vec2{ massData.center.x, massData.center.y },
				.momentOfInertia = massData.I,
			};
		}
	}
}