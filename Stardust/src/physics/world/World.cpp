#include "stardust/physics/world/World.h"

#include <memory>
#include <utility>

#include "stardust/ecs/components/RigidBodyComponent.h"
#include "stardust/ecs/entity/Entity.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace physics
    {
        namespace
        {
            class RaycastCallback final
                : public b2RayCastCallback
            {
            private:
                const Vector2 m_origin;
                CollisionLayer m_layerMask;

                bool& m_hasHitFixture;
                RaycastHit& m_raycastHitData;

                const World& m_world;

            public:
                inline RaycastCallback(const Vector2 origin, const CollisionLayer layerMask, bool& hasHitFixture, RaycastHit& raycastHitData, const World& world)
                    : m_origin(origin), m_layerMask(layerMask), m_hasHitFixture(hasHitFixture), m_raycastHitData(raycastHitData), m_world(world)
                { }

                virtual ~RaycastCallback() noexcept override = default;

                [[nodiscard]] inline virtual auto ReportFixture(b2Fixture* const fixture, const b2Vec2& point, const b2Vec2& normal, const f32 fraction) -> f32 override
                {
                    if (fixture->GetFilterData().categoryBits & m_layerMask)
                    {
                        m_hasHitFixture = true;

                        m_raycastHitData.collider = Collider(fixture, m_world);
                        m_raycastHitData.entityHandle = m_raycastHitData.collider.GetOwningBody()->GetAssociatedEntityHandle();

                        m_raycastHitData.fraction = fraction;
                        m_raycastHitData.normal = Vector2{ normal.x, normal.y };
                        m_raycastHitData.point = Vector2{ point.x, point.y };

                        m_raycastHitData.distance = glm::distance(m_origin, m_raycastHitData.point);

                        return fraction;
                    }
                    else
                    {
                        return -1.0f;
                    }
                }
            };

            class RaycastAllCallback final
                : public b2RayCastCallback
            {
            private:
                const Vector2 m_origin;
                CollisionLayer m_layerMask;

                List<RaycastHit>& m_raycastHits;

                const World& m_world;

            public:
                inline RaycastAllCallback(const Vector2 origin, const CollisionLayer layerMask, List<RaycastHit>& raycastHits, const World& world)
                    : m_origin(origin), m_layerMask(layerMask), m_raycastHits(raycastHits), m_world(world)
                { }

                virtual ~RaycastAllCallback() noexcept override = default;

                [[nodiscard]] inline virtual auto ReportFixture(b2Fixture* const fixture, const b2Vec2& point, const b2Vec2& normal, const f32 fraction) -> f32 override
                {
                    if (fixture->GetFilterData().categoryBits & m_layerMask)
                    {
                        m_raycastHits.emplace_back();

                        m_raycastHits.back().collider = Collider(fixture, m_world);
                        m_raycastHits.back().entityHandle = m_raycastHits.back().collider.GetOwningBody()->GetAssociatedEntityHandle();

                        m_raycastHits.back().fraction = fraction;
                        m_raycastHits.back().normal = Vector2{ normal.x, normal.y };
                        m_raycastHits.back().point = Vector2{ point.x, point.y };
                        m_raycastHits.back().distance = glm::distance(m_origin, m_raycastHits.back().point);

                        return 1.0f;
                    }
                    else
                    {
                        return -1.0f;
                    }
                }
            };

            class OverlapBoxCallback final
                : public b2QueryCallback
            {
            private:
                CollisionLayer m_layerMask;
                Collider& m_hitCollider;

                const World& m_world;

            public:
                inline OverlapBoxCallback(Collider& hitCollider, const CollisionLayer layerMask, const World& world)
                    : m_hitCollider(hitCollider), m_layerMask(layerMask), m_world(world)
                { }

                virtual ~OverlapBoxCallback() noexcept override = default;

                [[nodiscard]] inline virtual auto ReportFixture(b2Fixture* const fixture) -> bool override
                {
                    if (fixture->GetFilterData().categoryBits & m_layerMask)
                    {
                        m_hitCollider = Collider(fixture, m_world);

                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            };

            class OverlapBoxAllCallback final
                : public b2QueryCallback
            {
            private:
                CollisionLayer m_layerMask;
                List<Collider>& m_hitColliders;

                const World& m_world;

            public:
                inline OverlapBoxAllCallback(List<Collider>& hitColliders, const CollisionLayer layerMask, const World& world)
                    : m_hitColliders(hitColliders), m_layerMask(layerMask), m_world(world)
                { }

                virtual ~OverlapBoxAllCallback() noexcept override = default;

                [[nodiscard]] inline virtual auto ReportFixture(b2Fixture* const fixture) -> bool override
                {
                    if (fixture->GetFilterData().categoryBits & m_layerMask)
                    {
                        m_hitColliders.emplace_back(fixture, m_world);
                    }

                    return true;
                }
            };
        }

        World::CollisionListener::CollisionListener(const World& world)
            : m_world(world)
        { }

        auto World::CollisionListener::BeginContact(b2Contact* const contact) -> void
        {
            const ObserverPointer<b2Body> firstBody = contact->GetFixtureA()->GetBody();
            const ObserverPointer<b2Body> secondBody = contact->GetFixtureB()->GetBody();

            const bool isSensorContact = contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor();

            if (isSensorContact)
            {
                if (m_world.m_sensorEnterCallbacks.contains(firstBody))
                {
                    m_world.m_sensorEnterCallbacks.at(firstBody)(Collider(contact->GetFixtureB(), m_world));
                }
            }
            else
            {
                if (m_world.m_collisionEnterCallbacks.contains(firstBody))
                {
                    m_world.m_collisionEnterCallbacks.at(firstBody)(Collider(contact->GetFixtureB(), m_world));
                }
            }
        }

        auto World::CollisionListener::EndContact(b2Contact* const contact) -> void
        {
            const ObserverPointer<b2Body> firstBody = contact->GetFixtureA()->GetBody();
            const ObserverPointer<b2Body> secondBody = contact->GetFixtureB()->GetBody();

            const bool isSensorContact = contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor();

            if (isSensorContact)
            {
                if (m_world.m_sensorExitCallbacks.contains(firstBody))
                {
                    m_world.m_sensorExitCallbacks.at(firstBody)(Collider(contact->GetFixtureB(), m_world));
                }
            }
            else
            {
                if (m_world.m_collisionExitCallbacks.contains(firstBody))
                {
                    m_world.m_collisionExitCallbacks.at(firstBody)(Collider(contact->GetFixtureB(), m_world));
                }
            }
        }

        World::World(Scene& scene, const Vector2 gravity)
        {
            Initialise(scene, gravity);
        }

        auto World::Initialise(Scene& scene, const Vector2 gravity) -> void
        {
            m_handle = std::make_unique<b2World>(b2Vec2{ gravity.x, gravity.y });
            m_handle->SetContactListener(&m_collisionListener);

            m_scene = &scene;
        }

        auto World::Step(const f64 timestep) const -> void
        {
            m_handle->Step(static_cast<f32>(timestep), static_cast<i32>(s_velocityIterations), static_cast<i32>(s_positionIterations));
        }

        auto World::CreateBody(const Body::CreateInfo& createInfo, const ObserverPointer<EntityBundle> entityBundle) -> ObserverPointer<Body>
        {
            Entity entity = m_scene->CreateEntity(entityBundle);
            Body body(*this, createInfo, entity.GetHandle());

            const ObserverPointer<const b2Body> rawBodyHandle = body.GetRawHandle();
            m_bodies[rawBodyHandle] = std::move(body);

            entity.AddComponent<components::RigidBody>(&m_bodies[rawBodyHandle]);

            return &m_bodies[rawBodyHandle];
        }

        auto World::DestroyBody(const ObserverPointer<const Body> body) noexcept -> void
        {
            m_bodies.erase(body->GetRawHandle());
            m_handle->DestroyBody(body->GetRawHandle());
        }

        [[nodiscard]] auto World::LookupBody(const ObserverPointer<const b2Body> bodyHandle) -> ObserverPointer<Body>
        {
            if (m_bodies.contains(bodyHandle))
            {
                return &m_bodies.at(bodyHandle);
            }
            else
            {
                return nullptr;
            }
        }

        [[nodiscard]] auto World::LookupBody(const ObserverPointer<const b2Body> bodyHandle) const -> ObserverPointer<const Body>
        {
            if (m_bodies.contains(bodyHandle))
            {
                return &m_bodies.at(bodyHandle);
            }
            else
            {
                return nullptr;
            }
        }

        [[nodiscard]] auto World::Raycast(const Vector2 origin, const Vector2 direction, const f32 distance, const CollisionLayer layerMask) const -> Optional<RaycastHit>
        {
            if (distance == 0.0f || direction == Vector2Zero) [[unlikely]]
            {
                return None;
            }

            const Vector2 normalisedDirection = glm::normalize(direction);
            const Vector2 destinationPoint = origin + normalisedDirection * distance;

            if (origin == destinationPoint)
            {
                return None;
            }

            bool hasHitAnything = false;
            RaycastHit raycastHitData{ };

            RaycastCallback callback(origin, layerMask, hasHitAnything, raycastHitData, *this);
            m_handle->RayCast(&callback, b2Vec2{ origin.x, origin.y }, b2Vec2{ destinationPoint.x, destinationPoint.y });

            if (hasHitAnything)
            {
                return raycastHitData;
            }
            else
            {
                return None;
            }
        }

        [[nodiscard]] auto World::RaycastAll(const Vector2 origin, const Vector2 direction, const f32 distance, const CollisionLayer layerMask) const -> List<RaycastHit>
        {
            if (distance == 0.0f || direction == Vector2Zero) [[unlikely]]
            {
                return { };
            }

            const Vector2 normalisedDirection = glm::normalize(direction);
            const Vector2 destinationPoint = origin + normalisedDirection * distance;

            if (origin == destinationPoint)
            {
                return { };
            }

            List<RaycastHit> raycastHits{ };
            RaycastAllCallback callback(origin, layerMask, raycastHits, *this);
            m_handle->RayCast(&callback, b2Vec2{ origin.x, origin.y }, b2Vec2{ destinationPoint.x, destinationPoint.y });

            return raycastHits;
        }

        [[nodiscard]] auto World::RaycastBox(const AABB& box, const CollisionLayer layerMask) const -> bool
        {
            if (const auto upRaycastHit = Raycast(box.GetLowerBound(), Vector2Up, box.GetSize().y, layerMask); 
                upRaycastHit.has_value())
            {
                return true;
            }

            if (const auto downRaycastHit = Raycast(box.GetUpperBound(), Vector2Down, box.GetSize().y, layerMask); 
                downRaycastHit.has_value())
            {
                return true;
            }

            if (const auto leftRaycastHit = Raycast(Vector2{ box.GetUpperBound().x, box.GetLowerBound().y }, Vector2Left, box.GetSize().x, layerMask);
                leftRaycastHit.has_value())
            {
                return true;
            }

            const auto rightRaycastHit = Raycast(Vector2{ box.GetLowerBound().x, box.GetUpperBound().y }, Vector2Right, box.GetSize().x, layerMask);

            return rightRaycastHit.has_value();
        }

        [[nodiscard]] auto World::RaycastBox(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask) const -> bool
        {
            return RaycastBox(AABB(centre, halfSize), layerMask);
        }

        [[nodiscard]] auto World::RaycastBoxAll(const AABB& box, const CollisionLayer layerMask) const -> HashSet<Collider>
        {
            HashSet<Collider> overlappingColliders{ };

            for (const auto& upRaycastHit : RaycastAll(box.GetLowerBound(), Vector2Up, box.GetSize().y, layerMask))
            {
                overlappingColliders.insert(upRaycastHit.collider);
            }

            for (const auto& downRaycastHit : RaycastAll(box.GetUpperBound(), Vector2Down, box.GetSize().y, layerMask))
            {
                overlappingColliders.insert(downRaycastHit.collider);
            }

            for (const auto& leftRaycastHit : RaycastAll(Vector2{ box.GetUpperBound().x, box.GetLowerBound().y }, Vector2Left, box.GetSize().x, layerMask))
            {
                overlappingColliders.insert(leftRaycastHit.collider);
            }

            for (const auto& rightRaycastHit : RaycastAll(Vector2{ box.GetLowerBound().x, box.GetUpperBound().y }, Vector2Right, box.GetSize().x, layerMask))
            {
                overlappingColliders.insert(rightRaycastHit.collider);
            }

            return overlappingColliders;
        }

        [[nodiscard]] auto World::RaycastBoxAll(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask) const -> HashSet<Collider>
        {
            return RaycastBoxAll(AABB(centre, halfSize), layerMask);
        }

        [[nodiscard]] auto World::QueryBox(const AABB& box, const CollisionLayer layerMask) const -> Optional<Collider>
        {
            Collider hitFixture;
            OverlapBoxCallback callback(hitFixture, layerMask, *this);

            m_handle->QueryAABB(&callback, box);

            if (hitFixture.IsValid())
            {
                return hitFixture;
            }
            else
            {
                return None;
            }
        }

        [[nodiscard]] auto World::QueryBox(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask) const -> Optional<Collider>
        {
            const AABB box(centre, halfSize);

            Collider hitFixture;
            OverlapBoxCallback callback(hitFixture, layerMask, *this);

            m_handle->QueryAABB(&callback, box);

            if (hitFixture.IsValid())
            {
                return hitFixture;
            }
            else
            {
                return None;
            }
        }

        [[nodiscard]] auto World::QueryBoxAll(const AABB& box, const CollisionLayer layerMask) const -> List<Collider>
        {
            List<Collider> hitFixtures{ };
            OverlapBoxAllCallback callback(hitFixtures, layerMask, *this);

            m_handle->QueryAABB(&callback, box);

            return hitFixtures;
        }

        [[nodiscard]] auto World::QueryBoxAll(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask) const -> List<Collider>
        {
            const AABB box(centre, halfSize);

            List<Collider> hitFixtures{ };
            OverlapBoxAllCallback callback(hitFixtures, layerMask, *this);

            m_handle->QueryAABB(&callback, box);

            return hitFixtures;
        }

        [[nodiscard]] auto World::GetGravity() const -> Vector2
        {
            const b2Vec2 gravity = m_handle->GetGravity();

            return Vector2{ gravity.x, gravity.y };
        }

        auto World::SetGravity(const Vector2 gravity) const -> void
        {
            m_handle->SetGravity(b2Vec2{ gravity.x, gravity.y });
        }

        [[nodiscard]] auto World::IsSleepingAllowed() const -> bool
        {
            return m_handle->GetAllowSleeping();
        }

        auto World::AllowSleeping(const bool allowSleeping) const -> void
        {
            return m_handle->SetAllowSleeping(allowSleeping);
        }
    }
}
