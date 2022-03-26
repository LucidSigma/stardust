#include "stardust/physics/geometry/Edge.h"

#include "stardust/math/Math.h"

namespace stardust
{
    namespace physics
    {
        Edge::Edge(const b2EdgeShape& shapeHandle)
            : m_edge(shapeHandle)
        { }

        Edge::Edge(const Vector2 vertexA, const Vector2 vertexB)
        {
            SetTwoSided(vertexA, vertexB);
        }

        Edge::Edge(const Vector2 ghostVertexA, const Vector2 vertexA, const Vector2 vertexB, const Vector2 ghostVertexB)
        {
            SetOneSided(ghostVertexA, vertexA, vertexB, ghostVertexB);
        }

        auto Edge::SetOneSided(const Vector2 ghostVertexA, const Vector2 vertexA, const Vector2 vertexB, const Vector2 ghostVertexB) -> void
        {
            m_edge.SetOneSided(
                b2Vec2{ ghostVertexA.x, ghostVertexA.y },
                b2Vec2{ vertexA.x, vertexA.y },
                b2Vec2{ vertexB.x, vertexB.y },
                b2Vec2{ ghostVertexB.x, ghostVertexB.y }
            );
        }

        auto Edge::SetTwoSided(const Vector2 vertexA, const Vector2 vertexB) -> void
        {
            m_edge.SetTwoSided(
                b2Vec2{ vertexA.x, vertexA.y },
                b2Vec2{ vertexB.x, vertexB.y }
            );
        }

        [[nodiscard]] auto Edge::GetVertices() const -> Pair<Vector2, Vector2>
        {
            const Vector2 vertexA{ m_edge.m_vertex1.x, m_edge.m_vertex1.y };
            const Vector2 vertexB{ m_edge.m_vertex2.x, m_edge.m_vertex2.y };

            return  Pair<Vector2, Vector2>{ vertexA, vertexB };
        }

        [[nodiscard]] auto Edge::GetGhostVertices() const -> Pair<Vector2, Vector2>
        {
            const Vector2 ghostVertexA{ m_edge.m_vertex0.x, m_edge.m_vertex0.y };
            const Vector2 ghostVertexB{ m_edge.m_vertex3.x, m_edge.m_vertex3.y };

            return  Pair<Vector2, Vector2>{ ghostVertexA, ghostVertexB };
        }

        [[nodiscard]] auto Edge::ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            b2AABB aabb{ };
            m_edge.ComputeAABB(&aabb, transform, 0);

            return aabb;
        }

        [[nodiscard]] auto Edge::ComputeMassData(const f32 density) const -> MassData
        {
            b2MassData massData{ };
            m_edge.ComputeMass(&massData, density);

            return MassData{
                .mass = massData.mass,
                .centre = Vector2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }
    }
}
