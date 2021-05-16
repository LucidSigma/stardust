#include "stardust/physics/world/World.h"

#include <utility>

#include "stardust/math/Math.h"

namespace stardust
{
    namespace physics
    {
        namespace
        {
            class RaycastCallback
                : public b2RayCastCallback
            {
            private:
                const Vec2& m_origin;
                CollisionLayer m_layerMask;

                bool& m_hasHitFixture;
                RaycastHit& m_raycastHitData;

                const World& m_world;

            public:
                inline RaycastCallback(const Vec2& origin, const CollisionLayer layerMask, bool& hasHitFixture, RaycastHit& raycastHitData, const World& world)
                    : m_origin(origin), m_layerMask(layerMask), m_hasHitFixture(hasHitFixture), m_raycastHitData(raycastHitData), m_world(world)
                { }

                virtual ~RaycastCallback() noexcept override = default;

                inline virtual f32 ReportFixture(b2Fixture* const fixture, const b2Vec2& point, const b2Vec2& normal, const f32 fraction) override
                {
                    if (fixture->GetFilterData().categoryBits & m_layerMask)
                    {
                        m_hasHitFixture = true;

                        m_raycastHitData.collider = Collider(fixture, m_world);
                        m_raycastHitData.fraction = fraction;
                        m_raycastHitData.normal = Vec2{ normal.x, normal.y };
                        m_raycastHitData.point = Vec2{ point.x, point.y };

                        m_raycastHitData.distance = glm::distance(m_origin, m_raycastHitData.point);

                        return fraction;
                    }
                    else
                    {
                        return -1.0f;
                    }
                }
            };

            class RaycastAllCallback
                : public b2RayCastCallback
            {
            private:
                const Vec2& m_origin;
                CollisionLayer m_layerMask;

                Vector<RaycastHit>& m_raycastHits;

                const World& m_world;

            public:
                inline RaycastAllCallback(const Vec2& origin, const CollisionLayer layerMask, Vector<RaycastHit>& raycastHits, const World& world)
                    : m_origin(origin), m_layerMask(layerMask), m_raycastHits(raycastHits), m_world(world)
                { }

                virtual ~RaycastAllCallback() noexcept override = default;

                inline virtual f32 ReportFixture(b2Fixture* const fixture, const b2Vec2& point, const b2Vec2& normal, const f32 fraction) override
                {
                    if (fixture->GetFilterData().categoryBits & m_layerMask)
                    {
                        m_raycastHits.emplace_back();
                        m_raycastHits.back().collider = Collider(fixture, m_world);
                        m_raycastHits.back().fraction = fraction;
                        m_raycastHits.back().normal = Vec2{ normal.x, normal.y };
                        m_raycastHits.back().point = Vec2{ point.x, point.y };
                        m_raycastHits.back().distance = glm::distance(m_origin, m_raycastHits.back().point);

                        return 1.0f;
                    }
                    else
                    {
                        return -1.0f;
                    }
                }
            };

            class OverlapBoxCallback
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

                inline virtual bool ReportFixture(b2Fixture* const fixture) override
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

            class OverlapBoxAllCallback
                : public b2QueryCallback
            {
            private:
                CollisionLayer m_layerMask;
                Vector<Collider>& m_hitColliders;

                const World& m_world;

            public:
                inline OverlapBoxAllCallback(Vector<Collider>& hitColliders, const CollisionLayer layerMask, const World& world)
                    : m_hitColliders(hitColliders), m_layerMask(layerMask), m_world(world)
                { }

                virtual ~OverlapBoxAllCallback() noexcept override = default;

                inline virtual bool ReportFixture(b2Fixture* const fixture) override
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

        void World::CollisionListener::BeginContact(b2Contact* const contact)
        {
            const ObserverPtr<b2Body> firstBody = contact->GetFixtureA()->GetBody();
            const ObserverPtr<b2Body> secondBody = contact->GetFixtureB()->GetBody();

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

        void World::CollisionListener::EndContact(b2Contact* const contact)
        {
            const ObserverPtr<b2Body> firstBody = contact->GetFixtureA()->GetBody();
            const ObserverPtr<b2Body> secondBody = contact->GetFixtureB()->GetBody();

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

        World::World()
            : m_handle(std::make_unique<b2World>(b2Vec2_zero))
        {
            m_handle->SetContactListener(&m_collisionListener);
        }

        World::World(const Vec2& gravity)
            : m_handle(std::make_unique<b2World>(b2Vec2{ gravity.x, gravity.y }))
        {
            m_handle->SetContactListener(&m_collisionListener);
        }

        void World::Step(const f32 timestep) const
        {
            m_handle->Step(timestep, static_cast<i32>(s_velocityIterations), static_cast<i32>(s_positionIterations));
        }

        ObserverPtr<Body> World::CreateBody(const Body::CreateInfo& createInfo)
        {
            Body body(*this, createInfo);
            const b2Body* rawBodyHandle = body.GetRawHandle();
            m_bodies[rawBodyHandle] = std::move(body);

            return &m_bodies[rawBodyHandle];
        }

        void World::DestroyBody(ObserverPtr<const Body> body) noexcept
        {
            m_bodies.erase(body->GetRawHandle());
            m_handle->DestroyBody(body->GetRawHandle());
        }

        [[nodiscard]] ObserverPtr<Body> World::LookupBody(const ObserverPtr<const b2Body> bodyHandle)
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

        [[nodiscard]] ObserverPtr<const Body> World::LookupBody(const ObserverPtr<const b2Body> bodyHandle) const
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

        [[nodiscard]] Optional<RaycastHit> World::Raycast(const Vec2& origin, const Vec2& direction, const f32 distance, const CollisionLayer layerMask) const
        {
            const Vec2 normalisedDirection = glm::normalize(direction);
            const Vec2 destinationPoint = origin + normalisedDirection * distance;

            if (origin == destinationPoint)
            {
                return NullOpt;
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
                return NullOpt;
            }
        }

        [[nodiscard]] Vector<RaycastHit> World::RaycastAll(const Vec2& origin, const Vec2& direction, const f32 distance, const CollisionLayer layerMask) const
        {
            const Vec2 normalisedDirection = glm::normalize(direction);
            const Vec2 destinationPoint = origin + normalisedDirection * distance;

            if (origin == destinationPoint)
            {
                return { };
            }

            Vector<RaycastHit> raycastHits{ };
            RaycastAllCallback callback(origin, layerMask, raycastHits, *this);
            m_handle->RayCast(&callback, b2Vec2{ origin.x, origin.y }, b2Vec2{ destinationPoint.x, destinationPoint.y });

            return raycastHits;
        }

        [[nodiscard]] bool World::RaycastBox(const AABB& box, const CollisionLayer layerMask) const
        {
            const auto upRaycastHit = Raycast(box.GetLowerBound(), Vec2Up, box.GetSize().y, layerMask);

            if (upRaycastHit.has_value())
            {
                return true;
            }

            const auto downRaycastHit = Raycast(box.GetUpperBound(), Vec2Down, box.GetSize().y, layerMask);

            if (downRaycastHit.has_value())
            {
                return true;
            }

            const auto leftRaycastHit = Raycast(Vec2{ box.GetUpperBound().x, box.GetLowerBound().y }, Vec2Left, box.GetSize().x, layerMask);

            if (downRaycastHit.has_value())
            {
                return true;
            }

            const auto rightRaycastHit = Raycast(Vec2{ box.GetLowerBound().x, box.GetUpperBound().y }, Vec2Right, box.GetSize().x, layerMask);

            return rightRaycastHit.has_value();
        }

        [[nodiscard]] bool World::RaycastBox(const Vec2& centre, const Vec2& halfSize, const CollisionLayer layerMask) const
        {
            return RaycastBox(AABB(centre, halfSize), layerMask);
        }

        [[nodiscard]] Optional<Collider> World::QueryBox(const AABB& box, const CollisionLayer layerMask) const
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
                return NullOpt;
            }
        }

        [[nodiscard]] Optional<Collider> World::QueryBox(const Vec2& centre, const Vec2& halfSize, const CollisionLayer layerMask) const
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
                return NullOpt;
            }
        }

        [[nodiscard]] Vector<Collider> World::QueryBoxAll(const AABB& box, const CollisionLayer layerMask) const
        {
            Vector<Collider> hitFixtures{ };
            OverlapBoxAllCallback callback(hitFixtures, layerMask, *this);

            m_handle->QueryAABB(&callback, box);

            return hitFixtures;
        }

        [[nodiscard]] Vector<Collider> World::QueryBoxAll(const Vec2& centre, const Vec2& halfSize, const CollisionLayer layerMask) const
        {
            const AABB box(centre, halfSize);

            Vector<Collider> hitFixtures{ };
            OverlapBoxAllCallback callback(hitFixtures, layerMask, *this);

            m_handle->QueryAABB(&callback, box);

            return hitFixtures;
        }

        [[nodiscard]] Vec2 World::GetGravity() const
        {
            const b2Vec2 gravity = m_handle->GetGravity();

            return Vec2{ gravity.x, gravity.y };
        }

        void World::SetGravity(const Vec2& gravity) const
        {
            m_handle->SetGravity(b2Vec2{ gravity.x, gravity.y });
        }

        bool World::AllowsSleeping() const
        {
            return m_handle->GetAllowSleeping();
        }

        void World::AllowSleeping(const bool allowSleeping) const
        {
            return m_handle->SetAllowSleeping(allowSleeping);
        }
    }
}