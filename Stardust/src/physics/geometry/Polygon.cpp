#include "stardust/physics/geometry/Polygon.h"

#include "stardust/math/Math.h"

namespace stardust
{
    namespace physics
    {
        Polygon::Polygon(const b2PolygonShape& shapeHandle)
            : m_polygon(shapeHandle)
        { }

        Polygon::Polygon(const List<Vector2>& points)
        {
            Set(points);
        }

        auto Polygon::Set(const List<Vector2>& vertices) -> void
        {
            if (vertices.size() > GetMaxVertices())
            {
                return;
            }

            List<b2Vec2> convertedVertices(vertices.size());

            for (usize i = 0u; i < vertices.size(); ++i)
            {
                convertedVertices[i] = b2Vec2{ vertices[i].x, vertices[i].y };
            }

            m_polygon.Set(convertedVertices.data(), static_cast<i32>(convertedVertices.size()));
        }

        auto Polygon::SetAsBox(const f32 width, const f32 height) -> void
        {
            m_polygon.SetAsBox(width / 2.0f, height / 2.0f);
        }

        auto Polygon::SetAsBox(const f32 width, const f32 height, const Vector2 pivot, const f32 rotation) -> void
        {
            m_polygon.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2{ pivot.x, pivot.y }, -glm::radians(rotation));
        }

        [[nodiscard]] auto Polygon::GetCentroid() const noexcept -> Vector2
        {
            return Vector2{ m_polygon.m_centroid.x, m_polygon.m_centroid.y };
        }

        [[nodiscard]] auto Polygon::GetVertices() const -> List<Vector2>
        {
            List<Vector2> vertices(GetVertexCount());

            for (usize i = 0u; i < vertices.size(); ++i)
            {
                vertices[i] = Vector2{ m_polygon.m_vertices[i].x, m_polygon.m_vertices[i].y };
            }

            return vertices;
        }

        [[nodiscard]] auto Polygon::GetNormals() const -> List<Vector2>
        {
            List<Vector2> normals(GetVertexCount());

            for (usize i = 0u; i < normals.size(); ++i)
            {
                normals[i] = Vector2{ m_polygon.m_normals[i].x, m_polygon.m_normals[i].y };
            }

            return normals;
        }

        [[nodiscard]] auto Polygon::ContainsPoint(const Vector2 worldPosition, const f32 rotation, const Vector2 point) const -> bool
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            return m_polygon.TestPoint(transform, b2Vec2{ point.x, point.y });
        }

        [[nodiscard]] auto Polygon::ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            b2AABB aabb{ };
            m_polygon.ComputeAABB(&aabb, transform, 0);

            return aabb;
        }

        [[nodiscard]] auto Polygon::ComputeMassData(const f32 density) const -> MassData
        {
            b2MassData massData{ };
            m_polygon.ComputeMass(&massData, density);

            return MassData{
                .mass = massData.mass,
                .centre = Vector2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }

        [[nodiscard]] auto Polygon::IsConvex() const -> bool
        {
            return m_polygon.Validate();
        }
    }
}
