#pragma once
#ifndef STARDUST_BODY_H
#define STARDUST_BODY_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <box2d/box2d.h>

#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/math/Math.h"
#include "stardust/physics/collider/Collider.h"
#include "stardust/physics/Physics.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        class Body final
            : private INoncopyable
        {
        public:
            enum class Type
                : std::underlying_type_t<b2BodyType>
            {
                Static = b2_staticBody,
                Kinematic = b2_kinematicBody,
                Dynamic = b2_dynamicBody,
            };

            struct CreateInfo final
            {
                Type type = Type::Static;

                Vector2 position = Vector2Zero;
                f32 angle = 0.0f;

                Vector2 linearVelocity = Vector2Zero;
                f32 angularVelocity = 0.0f;

                f32 linearDamping = 0.0f;
                f32 angularDamping = 0.0f;

                bool allowSleep = true;
                bool isAwake = true;

                bool hasFixedRotation = false;
                bool isBullet = false;

                bool isEnabled = true;

                f32 gravityScale = 1.0f;

                List<Collider::CreateInfo> colliderInfos{ };
            };

        private:
            ObserverPointer<b2Body> m_handle = nullptr;
            ObserverPointer<const class World> m_owningWorld = nullptr;

            HashSet<Collider> m_colliders{ };

            EntityHandle m_associatedEntityHandle = NullEntityHandle;

        public:
            Body() = default;
            Body(const class World& world, const CreateInfo& createInfo, const EntityHandle associatedEntityHandle);
            Body(Body&& other) noexcept;
            auto operator =(Body&& other) noexcept -> Body&;
            ~Body() noexcept = default;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr; }

            auto ApplyForce(const Vector2 force, const Vector2 point, const bool wakeUp) const -> void;
            auto ApplyForceToCentre(const Vector2 force, const bool wakeUp) const -> void;
            auto ApplyTorque(const f32 torque, const bool wakeUp) const -> void;

            auto ApplyLinearImpulse(const Vector2 impulse, const Vector2 point, const bool wakeUp) const -> void;
            auto ApplyLinearImpulseToCentre(const Vector2 impulse, const bool wakeUp) const -> void;
            auto ApplyAngularImpulse(const f32 impulse, const bool wakeUp) const -> void;

            auto Move(const Vector2 positionOffset) const -> void;
            auto Rotate(const f32 angleOffset) const -> void;

            auto AddCollider(const Collider::CreateInfo& colliderInfo) -> Collider;
            auto RemoveCollider(const Collider& collider) -> void;
            [[nodiscard]] inline auto GetColliders() const -> const HashSet<Collider>& { return m_colliders; }
            [[nodiscard]] inline auto GetFirstCollider() const -> const Collider& { return *std::cbegin(m_colliders); }
            [[nodiscard]] inline auto HasColliders() const noexcept -> bool { return !m_colliders.empty(); }
            [[nodiscard]] inline auto HasCollider(const Collider& collider) const -> bool { return m_colliders.contains(collider); }

            [[nodiscard]] auto GetWorldCentre() const -> Vector2;
            [[nodiscard]] auto GetLocalCenter() const -> Vector2;

            [[nodiscard]] auto GetWorldPoint(const Vector2 localPoint) const -> Vector2;
            [[nodiscard]] auto GetWorldVector(const Vector2 localVector) const -> Vector2;
            [[nodiscard]] auto GetLocalPoint(const Vector2 worldPoint) const -> Vector2;
            [[nodiscard]] auto GetLocalVector(const Vector2 worldVector) const -> Vector2;
            [[nodiscard]] auto GetLinearVelocityFromWorldPoint(const Vector2 worldPoint) const -> Vector2;
            [[nodiscard]] auto GetLinearVelocityFromLocalPoint(const Vector2 localPoint) const -> Vector2;

            [[nodiscard]] auto IsEnabled() const -> bool;
            auto SetEnabled(const bool isEnabled) const -> void;
            [[nodiscard]] auto IsAwake() const -> bool;
            [[nodiscard]] auto CanSleep() const -> bool;
            auto SetAllowSleeping(const bool canSleep) const -> void;

            [[nodiscard]] auto GetType() const -> Type;
            auto SetType(const Type type) const -> void;

            [[nodiscard]] auto IsBullet() const -> bool;
            auto SetBullet(const bool isBullet) const -> void;

            [[nodiscard]] auto GetPosition() const -> Vector2;
            auto SetPosition(const Vector2 position) const -> void;
            [[nodiscard]] auto GetRotation() const -> f32;
            auto SetRotation(const f32 rotation) const -> void;

            [[nodiscard]] auto HasFixedRotation() const -> bool;
            auto SetFixedRotation(const bool hasFixedRotation) const -> void;

            [[nodiscard]] auto GetLinearVelocity() const -> Vector2;
            auto SetLinearVelocity(const Vector2 linearVelocity) const -> void;
            [[nodiscard]] auto GetAngularVelocity() const -> f32;
            auto SetAngularVelocity(const f32 angularVelocity) const -> void;

            [[nodiscard]] auto GetLinearDamping() const -> f32;
            auto SetLinearDamping(const f32 linearDamping) const -> void;
            [[nodiscard]] auto GetAngularDamping() const -> f32;
            auto SetAngularDamping(const f32 angularDamping) const -> void;

            [[nodiscard]] auto GetGravityScale() const -> f32;
            auto SetGravityScale(const f32 gravityScale) const -> void;

            [[nodiscard]] auto GetMass() const -> f32;
            [[nodiscard]] auto GetInertia() const -> f32;
            [[nodiscard]] auto GetMassData() const -> MassData;
            auto SetMassData(const MassData& massData) const -> void;
            auto ResetMassData() const -> void;

            [[nodiscard]] inline auto GetAssociatedEntityHandle() const noexcept -> EntityHandle { return m_associatedEntityHandle; }

            [[nodiscard]] inline auto GetRawHandle() const noexcept -> ObserverPointer<b2Body> { return m_handle; }
            [[nodiscard]] inline auto GetOwningWorld() const noexcept -> ObserverPointer<const class World> { return m_owningWorld; }

        private:
            auto Initialise(const class World& world, const CreateInfo& createInfo, const EntityHandle associatedEntityHandle) -> void;
        };
    }
}

#endif
