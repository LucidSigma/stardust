#include "stardust/physics/geometry/Chain.h"

#include "stardust/math/Math.h"

namespace stardust
{
	namespace physics
	{
		Chain::Chain(const b2ChainShape& shapeHandle)
			: m_chain(shapeHandle)
		{ }

		Chain::Chain(const Vector<Vec2>& vertices)
		{
			CreateLoop(vertices);
		}

		Chain::Chain(const Vector<Vec2>& vertices, const Vec2& ghostVertexA, const Vec2& ghostVertexB)
		{
			CreateChain(vertices, ghostVertexA, ghostVertexB);
		}

		void Chain::CreateLoop(const Vector<Vec2>& vertices)
		{
			Vector<b2Vec2> convertedVertices(vertices.size());

			for (usize i = 0u; i < vertices.size(); ++i)
			{
				convertedVertices[i] = b2Vec2{ vertices[i].x, vertices[i].y };
			}

			m_chain.CreateLoop(convertedVertices.data(), static_cast<i32>(convertedVertices.size()));
		}

		void Chain::CreateChain(const Vector<Vec2>& vertices, const Vec2& ghostVertexA, const Vec2& ghostVertexB)
		{
			Vector<b2Vec2> convertedVertices(vertices.size());

			for (usize i = 0u; i < vertices.size(); ++i)
			{
				convertedVertices[i] = b2Vec2{ vertices[i].x, vertices[i].y };
			}

			m_chain.CreateChain(
				convertedVertices.data(),
				static_cast<i32>(convertedVertices.size()),
				b2Vec2{ ghostVertexA.x, ghostVertexA.y },
				b2Vec2{ ghostVertexB.x, ghostVertexB.y }
			);
		}

		void Chain::Clear()
		{
			m_chain.Clear();
		}

		[[nodiscard]] Vector<Vec2> Chain::GetVertices() const
		{
			Vector<Vec2> vertices(GetVertexCount());

			for (usize i = 0u; i < vertices.size(); ++i)
			{
				vertices[i] = Vec2{ m_chain.m_vertices[i].x, m_chain.m_vertices[i].y };
			}

			return vertices;
		}

		[[nodiscard]] Edge Chain::GetChildEdge(const u32 childEdgeIndex) const
		{
			b2EdgeShape edge{ };
			m_chain.GetChildEdge(&edge, static_cast<i32>(childEdgeIndex));

			return edge;
		}

		[[nodiscard]] Pair<Vec2, Vec2> Chain::GetGhostVertices() const
		{
			const Vec2 ghostVertexA{ m_chain.m_prevVertex.x, m_chain.m_prevVertex.y };
			const Vec2 ghostVertexB{ m_chain.m_nextVertex.x, m_chain.m_nextVertex.y };

			return  Pair<Vec2, Vec2>{ ghostVertexA, ghostVertexB };
		}

		[[nodiscard]] bool Chain::TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			return m_chain.TestPoint(transform, b2Vec2{ point.x, point.y });
		}

		[[nodiscard]] AABB Chain::ComputeAABB(const Vec2& worldPosition, const f32 rotation) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			b2AABB aabb{ };
			m_chain.ComputeAABB(&aabb, transform, 0);

			return aabb;
		}

		[[nodiscard]] MassData Chain::ComputeMassData(const f32 density) const
		{
			b2MassData massData{ };
			m_chain.ComputeMass(&massData, density);

			return MassData{
				.mass = massData.mass,
				.centre = Vec2{ massData.center.x, massData.center.y },
				.momentOfInertia = massData.I,
			};
		}
	}
}