#include "stardust/physics/collider/Collider.h"

#include "stardust/physics/body/Body.h"
#include "stardust/physics/world/World.h"
#include "stardust/types/MathTypes.h"

namespace stardust
{
    namespace physics
    {
        Collider::Collider(const Body& body, const CreateInfo& createInfo)
            : m_owningBody(&body)
        {
            b2FixtureDef fixtureDef{ };
            fixtureDef.shape = createInfo.shape;
            fixtureDef.friction = createInfo.friction;
            fixtureDef.restitution = createInfo.restitution;
            fixtureDef.restitutionThreshold = createInfo.restitutionThreshold;
            fixtureDef.density = createInfo.density;
            fixtureDef.isSensor = createInfo.isSensor;
            fixtureDef.filter.categoryBits = createInfo.filter.layers;
            fixtureDef.filter.maskBits = createInfo.filter.collidesWith;
            fixtureDef.filter.groupIndex = static_cast<i32>(createInfo.filter.groupIndex);
            fixtureDef.userData = b2FixtureUserData{ };

            m_handle = body.GetRawHandle()->CreateFixture(&fixtureDef);
        }

        Collider::Collider(const ObserverPointer<b2Fixture> fixtureHandle, const World& world)
            : m_handle(fixtureHandle), m_owningBody(world.LookupBody(fixtureHandle->GetBody()))
        { }

        [[nodiscard]] auto Collider::GetShapeType() const -> ShapeType
        {
            return static_cast<ShapeType>(m_handle->GetType());
        }

        [[nodiscard]] auto Collider::GetShape() -> ObserverPointer<b2Shape>
        {
            return m_handle->GetShape();
        }

        [[nodiscard]] auto Collider::GetShape() const -> ObserverPointer<const b2Shape>
        {
            return m_handle->GetShape();
        }

        [[nodiscard]] auto Collider::GetVertexCount() const -> u32
        {
            switch (GetShapeType())
            {
            case ShapeType::Chain:
                return static_cast<u32>(static_cast<ObserverPointer<const b2ChainShape>>(GetShape())->m_count);

            case ShapeType::Polygon:
                return static_cast<u32>(static_cast<ObserverPointer<const b2PolygonShape>>(GetShape())->m_count);

            case ShapeType::Edge:
                return 2u;

            case ShapeType::Circle:
            [[unlikely]] default:
                return 0u;
            }
        }

        [[nodiscard]] auto Collider::GetVertices() const -> List<Vector2>
        {
            switch (GetShapeType())
            {
            case ShapeType::Chain:
            {
                const ObserverPointer<const b2Vec2> verticesPointer = static_cast<ObserverPointer<const b2ChainShape>>(GetShape())->m_vertices;
                const usize vertexCount = static_cast<usize>(GetVertexCount());

                List<Vector2> vertices(vertexCount);

                for (usize i = 0u; i < vertexCount; ++i)
                {
                    vertices[i] = Vector2{ verticesPointer[i].x, verticesPointer[i].y };
                }

                return vertices;
            }

            case ShapeType::Polygon:
            {
                const ObserverPointer<const b2Vec2> verticesPointer = static_cast<ObserverPointer<const b2PolygonShape>>(GetShape())->m_vertices;
                const usize vertexCount = static_cast<usize>(GetVertexCount());

                List<Vector2> vertices(vertexCount);

                for (usize i = 0u; i < vertexCount; ++i)
                {
                    vertices[i] = Vector2{ verticesPointer[i].x, verticesPointer[i].y };
                }

                return vertices;
            }

            case ShapeType::Edge:
            {
                const b2Vec2 vertexA = static_cast<ObserverPointer<const b2EdgeShape>>(GetShape())->m_vertex1;
                const b2Vec2 vertexB = static_cast<ObserverPointer<const b2EdgeShape>>(GetShape())->m_vertex2;

                return {
                    Vector2{ vertexA.x, vertexA.y },
                    Vector2{ vertexB.x, vertexB.y },
                };
            }

            case ShapeType::Circle:
            [[unlikely]] default:
                return { };
            }
        }

        [[nodiscard]] auto Collider::GetFriction() const -> f32
        {
            return m_handle->GetFriction();
        }

        auto Collider::SetFriction(const f32 friction) const -> void
        {
            m_handle->SetFriction(friction);
        }

        [[nodiscard]] auto Collider::GetRestitution() const -> f32
        {
            return m_handle->GetRestitution();
        }

        auto Collider::SetRestitution(const f32 restitution) const -> void
        {
            m_handle->SetRestitution(restitution);
        }

        [[nodiscard]] auto Collider::GetRestitutionThreshold() const -> f32
        {
            return m_handle->GetRestitutionThreshold();
        }

        auto Collider::SetRestitutionThreshold(const f32 restitutionThreshold) const -> void
        {
            m_handle->SetRestitutionThreshold(restitutionThreshold);
        }

        [[nodiscard]] auto Collider::GetDensity() const -> f32
        {
            return m_handle->GetDensity();
        }

        auto Collider::SetDensity(const f32 density) const -> void
        {
            m_handle->SetDensity(density);
        }

        [[nodiscard]] auto Collider::IsSensor() const -> bool
        {
            return m_handle->IsSensor();
        }

        auto Collider::SetSensor(const bool isSensor) const -> void
        {
            m_handle->SetSensor(isSensor);
        }

        [[nodiscard]] auto Collider::GetFilterData() const -> Filter
        {
            const b2Filter filterData = m_handle->GetFilterData();

            return Filter{
                .layers = filterData.categoryBits,
                .collidesWith = filterData.maskBits,
                .groupIndex = static_cast<GroupIndex>(filterData.groupIndex),
            };
        }

        auto Collider::SetFilterData(const Filter& filterData) const -> void
        {
            b2Filter filter;
            filter.categoryBits = filterData.layers;
            filter.maskBits = filterData.collidesWith;
            filter.groupIndex = static_cast<i32>(filterData.groupIndex);

            m_handle->SetFilterData(filter);
        }

        auto Collider::Refilter() const -> void
        {
            m_handle->Refilter();
        }

        [[nodiscard]] auto Collider::GetAABB() const -> AABB
        {
            return m_handle->GetAABB(0);
        }

        [[nodiscard]] auto Collider::GetMassData() const -> MassData
        {
            b2MassData massData{ };
            m_handle->GetMassData(&massData);

            return MassData{
                .mass = massData.mass,
                .centre = Vector2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }
    }
}
