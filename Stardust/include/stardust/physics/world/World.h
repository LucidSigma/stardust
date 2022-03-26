#pragma once
#ifndef STARDUST_WORLD_H
#define STARDUST_WORLD_H

#include <functional>

#include <box2d/box2d.h>

#include "stardust/ecs/bundle/EntityBundle.h"
#include "stardust/math/Math.h"
#include "stardust/physics/body/Body.h"
#include "stardust/physics/collider/Collider.h"
#include "stardust/physics/RaycastHit.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"
#include "stardust/scene/Scene.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        using CollisionCallback = std::function<void(const Collider& other)>;

        class World final
        {
        private:
            class CollisionListener final
                : public b2ContactListener
            {
            private:
                const World& m_world;

            public:
                explicit CollisionListener(const World& world);
                virtual ~CollisionListener() noexcept = default;

                virtual void BeginContact(b2Contact* const contact) override;
                virtual void EndContact(b2Contact* const contact) override;
            };

            inline static u32 s_velocityIterations = 8u;
            inline static u32 s_positionIterations = 3u;

            UniquePointer<b2World> m_handle = nullptr;
            ObserverPointer<Scene> m_scene = nullptr;

            HashMap<const b2Body*, Body> m_bodies{ };

            CollisionListener m_collisionListener{ *this };
            HashMap<const b2Body*, CollisionCallback> m_collisionEnterCallbacks{ };
            HashMap<const b2Body*, CollisionCallback> m_collisionExitCallbacks{ };
            HashMap<const b2Body*, CollisionCallback> m_sensorEnterCallbacks{ };
            HashMap<const b2Body*, CollisionCallback> m_sensorExitCallbacks{ };

        public:
            [[nodiscard]] inline static auto GetVelocityIterations() noexcept -> u32 { return s_velocityIterations; }
            inline static auto SetVelocityIterations(const u32 iterations) noexcept -> void { s_velocityIterations = iterations; }
            [[nodiscard]] inline static auto GetPositionIterations() noexcept -> u32 { return s_positionIterations; }
            inline static auto SetPositionIterations(const u32 iterations) noexcept -> void { s_positionIterations = iterations; }

            World() = default;
            World(Scene& scene, const Vector2 gravity = Vector2Zero);
            ~World() noexcept = default;

            auto Initialise(Scene& scene, const Vector2 gravity = Vector2Zero) -> void;

            auto Step(const f64 timestep) const -> void;

            auto CreateBody(const Body::CreateInfo& createInfo, const ObserverPointer<EntityBundle> entityBundle = nullptr) -> ObserverPointer<Body>;
            auto DestroyBody(const ObserverPointer<const Body> body) noexcept -> void;
            [[nodiscard]] auto LookupBody(const ObserverPointer<const b2Body> bodyHandle) -> ObserverPointer<Body>;
            [[nodiscard]] auto LookupBody(const ObserverPointer<const b2Body> bodyHandle) const -> ObserverPointer<const Body>;

            inline auto SetCollisionEnterCallback(const Body& body, const CollisionCallback& callback) -> void { m_collisionEnterCallbacks[body.GetRawHandle()] = callback; }
            inline auto SetCollisionExitCallback(const Body& body, const CollisionCallback& callback) -> void { m_collisionExitCallbacks[body.GetRawHandle()] = callback; }
            inline auto SetSensorEnterCallback(const Body& body, const CollisionCallback& callback) -> void { m_sensorEnterCallbacks[body.GetRawHandle()] = callback; }
            inline auto SetSensorExitCallback(const Body& body, const CollisionCallback& callback) -> void { m_sensorExitCallbacks[body.GetRawHandle()] = callback; }

            [[nodiscard]] auto Raycast(const Vector2 origin, const Vector2 direction, const f32 distance, const CollisionLayer layerMask = AllLayers) const -> Optional<RaycastHit>;
            [[nodiscard]] auto RaycastAll(const Vector2 origin, const Vector2 direction, const f32 distance, const CollisionLayer layerMask = AllLayers) const -> List<RaycastHit>;

            [[nodiscard]] auto RaycastBox(const AABB& box, const CollisionLayer layerMask = AllLayers) const -> bool;
            [[nodiscard]] auto RaycastBox(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask = AllLayers) const -> bool;
            [[nodiscard]] auto RaycastBoxAll(const AABB& box, const CollisionLayer layerMask = AllLayers) const -> HashSet<Collider>;
            [[nodiscard]] auto RaycastBoxAll(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask = AllLayers) const -> HashSet<Collider>;

            [[nodiscard]] auto QueryBox(const AABB& box, const CollisionLayer layerMask = AllLayers) const -> Optional<Collider>;
            [[nodiscard]] auto QueryBox(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask = AllLayers) const -> Optional<Collider>;
            [[nodiscard]] auto QueryBoxAll(const AABB& box, const CollisionLayer layerMask = AllLayers) const -> List<Collider>;
            [[nodiscard]] auto QueryBoxAll(const Vector2 centre, const Vector2 halfSize, const CollisionLayer layerMask = AllLayers) const -> List<Collider>;

            [[nodiscard]] auto GetGravity() const -> Vector2;
            auto SetGravity(const Vector2 gravity) const -> void;

            [[nodiscard]] auto IsSleepingAllowed() const -> bool;
            auto AllowSleeping(const bool allowSleeping) const -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr; }
            [[nodiscard]] inline auto GetRawHandle() const noexcept -> b2World* { return m_handle.get(); }
        };
    }
}

#endif
