#include "stardust/physics/geometry/Polygon.h"

#include "stardust/math/Math.h"

namespace stardust
{
	namespace physics
	{
		Polygon::Polygon(const Vector<Vec2>& points)
		{
			Set(points);
		}

		void Polygon::Set(const Vector<Vec2>& vertices)
		{
			if (vertices.size() < GetMaxVertices())
			{
				return;
			}

			Vector<b2Vec2> convertedVertices(vertices.size());

			for (usize i = 0u; i < vertices.size(); ++i)
			{
				convertedVertices[i] = b2Vec2{ vertices[i].x, vertices[i].y };
			}

			m_polygon.Set(convertedVertices.data(), static_cast<i32>(convertedVertices.size()));
		}

		void Polygon::SetAsBox(const f32 width, const f32 height)
		{
			m_polygon.SetAsBox(width / 2.0f, height / 2.0f);
		}

		void Polygon::SetAsBox(const f32 width, const f32 height, const Vec2& pivot, const f32 rotation)
		{
			m_polygon.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2{ pivot.x, pivot.y }, -glm::radians(rotation));
		}

		[[nodiscard]] Vec2 Polygon::GetCentroid() const noexcept
		{
			return Vec2{ m_polygon.m_centroid.x, m_polygon.m_centroid.y };
		}

		[[nodiscard]] Vector<Vec2> Polygon::GetVertices() const
		{
			Vector<Vec2> vertices(GetVertexCount());

			for (usize i = 0u; i < vertices.size(); ++i)
			{
				vertices[i] = Vec2{ m_polygon.m_vertices[i].x, m_polygon.m_vertices[i].y };
			}

			return vertices;
		}

		[[nodiscard]] Vector<Vec2> Polygon::GetNormals() const
		{
			Vector<Vec2> normals(GetVertexCount());

			for (usize i = 0u; i < normals.size(); ++i)
			{
				normals[i] = Vec2{ m_polygon.m_normals[i].x, m_polygon.m_normals[i].y };
			}

			return normals;
		}

		[[nodiscard]] bool Polygon::TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			return m_polygon.TestPoint(transform, b2Vec2{ point.x, point.y });
		}

		[[nodiscard]] AABB Polygon::ComputeAABB(const Vec2& worldPosition, const f32 rotation) const
		{
			b2Transform transform{ };
			transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

			b2AABB aabb{ };
			m_polygon.ComputeAABB(&aabb, transform, 0);

			return aabb;
		}

		[[nodiscard]] MassData Polygon::ComputeMassData(const f32 density) const
		{
			b2MassData massData{ };
			m_polygon.ComputeMass(&massData, density);

			return MassData{
				.mass = massData.mass,
				.centre = Vec2{ massData.center.x, massData.center.y },
				.momentOfInertia = massData.I,
			};
		}

		[[nodiscard]] bool Polygon::IsConvex() const
		{
			return m_polygon.Validate();
		}
	}
}