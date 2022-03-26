#include "stardust/physics/geometry/Chain.h"

#include "stardust/math/Math.h"

namespace stardust
{
    namespace physics
    {
        Chain::Chain(const b2ChainShape& shapeHandle)
            : m_chain(shapeHandle)
        { }

        Chain::Chain(const List<Vector2>& vertices)
        {
            CreateLoop(vertices);
        }

        Chain::Chain(const List<Vector2>& vertices, const Vector2 ghostVertexA, const Vector2 ghostVertexB)
        {
            CreateChain(vertices, ghostVertexA, ghostVertexB);
        }

        auto Chain::CreateLoop(const List<Vector2>& vertices) -> void
        {
            List<b2Vec2> convertedVertices(vertices.size());

            for (usize i = 0u; i < vertices.size(); ++i)
            {
                convertedVertices[i] = b2Vec2{ vertices[i].x, vertices[i].y };
            }

            m_chain.CreateLoop(convertedVertices.data(), static_cast<i32>(convertedVertices.size()));
        }

        auto Chain::CreateChain(const List<Vector2>& vertices, const Vector2 ghostVertexA, const Vector2 ghostVertexB) -> void
        {
            List<b2Vec2> convertedVertices(vertices.size());

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

        auto Chain::Clear() -> void
        {
            m_chain.Clear();
        }

        [[nodiscard]] auto Chain::GetVertices() const -> List<Vector2>
        {
            List<Vector2> vertices(GetVertexCount());

            for (usize i = 0u; i < vertices.size(); ++i)
            {
                vertices[i] = Vector2{ m_chain.m_vertices[i].x, m_chain.m_vertices[i].y };
            }

            return vertices;
        }

        [[nodiscard]] auto Chain::GetChildEdge(const u32 childEdgeIndex) const -> Edge
        {
            b2EdgeShape edge{ };
            m_chain.GetChildEdge(&edge, static_cast<i32>(childEdgeIndex));

            return edge;
        }

        [[nodiscard]] auto Chain::GetGhostVertices() const -> Pair<Vector2, Vector2>
        {
            const Vector2 ghostVertexA{ m_chain.m_prevVertex.x, m_chain.m_prevVertex.y };
            const Vector2 ghostVertexB{ m_chain.m_nextVertex.x, m_chain.m_nextVertex.y };

            return  Pair<Vector2, Vector2>{ ghostVertexA, ghostVertexB };
        }

        [[nodiscard]] auto Chain::ComputeEdgeAABB(const Vector2 worldPosition, const f32 rotation, const u32 childEdgeIndex) const -> AABB
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            b2AABB aabb{ };
            m_chain.ComputeAABB(&aabb, transform, static_cast<i32>(childEdgeIndex));

            return aabb;
        }

        [[nodiscard]] auto Chain::ComputeMassData(const f32 density) const -> MassData
        {
            b2MassData massData{ };
            m_chain.ComputeMass(&massData, density);

            return MassData{
                .mass = massData.mass,
                .centre = Vector2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }
    }
}
