#pragma once
#ifndef STARDUST_BODY_H
#define STARDUST_BODY_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/math/Math.h"
#include "stardust/physics/collider/Collider.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
    namespace physics
    {
        class Body
            : private INoncopyable
        {
        public:
            enum Type
                : std::underlying_type_t<b2BodyType>
            {
                Static = b2_staticBody,
                Kinematic = b2_kinematicBody,
                Dynamic = b2_dynamicBody,
            };

            struct CreateInfo
            {
                Type type = Type::Static;

                Vec2 position = Vec2Zero;
                f32 angle = 0.0f;

                Vec2 linearVelocity = Vec2Zero;
                f32 angularVelocity = 0.0f;

                f32 linearDamping = 0.0f;
                f32 angularDamping = 0.0f;

                bool allowSleep = true;
                bool isAwake = true;

                bool hasFixedRotation = false;
                bool isBullet = false;

                bool isEnabled = true;

                f32 gravityScale = 1.0f;

                Vector<Collider::CreateInfo> colliderInfos{ };
            };

        private:
            ObserverPtr<b2Body> m_handle = nullptr;
            ObserverPtr<const class World> m_owningWorld = nullptr;

            HashSet<Collider> m_colliders{ };

        public:
            Body() = default;
            Body(const class World& world, const CreateInfo& createInfo);
            Body(Body&& other) noexcept;
            Body& operator =(Body&& other) noexcept;
            ~Body() noexcept = default;

            void Initialise(const class World& world, const CreateInfo& createInfo);

            [[nodiscard]] inline bool IsValid() const noexcept { return m_handle != nullptr; }

            void ApplyForce(const Vec2& force, const Vec2& point, const bool wakeUp) const;
            void ApplyForceToCentre(const Vec2& force, const bool wakeUp) const;
            void ApplyTorque(const f32 torque, const bool wakeUp) const;

            void ApplyLinearImpulse(const Vec2& impulse, const Vec2& point, const bool wakeUp) const;
            void ApplyLinearImpulseToCentre(const Vec2& impulse, const bool wakeUp) const;
            void ApplyAngularImpulse(const f32 impulse, const bool wakeUp) const;

            void Move(const Vec2& positionOffset) const;
            void Rotate(const f32 angleOffset) const;

            Collider AddCollider(const Collider::CreateInfo& colliderInfo);
            void RemoveCollider(const Collider& collider);
            [[nodiscard]] inline const HashSet<Collider>& GetColliders() const { return m_colliders; }
            [[nodiscard]] inline const Collider& GetFirstCollider() const { return *std::cbegin(m_colliders); }
            [[nodiscard]] inline bool HasColliders() const noexcept { return !m_colliders.empty(); }
            [[nodiscard]] inline bool HasCollider(const Collider& collider) const { return m_colliders.contains(collider); }

            [[nodiscard]] Vec2 GetWorldCentre() const;
            [[nodiscard]] Vec2 GetLocalCenter() const;

            [[nodiscard]] Vec2 GetWorldPoint(const Vec2& localPoint) const;
            [[nodiscard]] Vec2 GetWorldVector(const Vec2& localVector) const;
            [[nodiscard]] Vec2 GetLocalPoint(const Vec2& worldPoint) const;
            [[nodiscard]] Vec2 GetLocalVector(const Vec2& worldVector) const;
            [[nodiscard]] Vec2 GetLinearVelocityFromWorldPoint(const Vec2& worldPoint) const;
            [[nodiscard]] Vec2 GetLinearVelocityFromLocalPoint(const Vec2& localPoint) const;

            [[nodiscard]] bool IsEnabled() const;
            void SetEnabled(const bool isEnabled) const;
            [[nodiscard]] bool IsAwake() const;
            [[nodiscard]] bool CanSleep() const;
            void SetAllowSleeping(const bool canSleep) const;

            [[nodiscard]] Type GetType() const;
            void SetType(const Type type) const;

            [[nodiscard]] bool IsBullet() const;
            void SetBullet(const bool isBullet) const;

            [[nodiscard]] Vec2 GetPosition() const;
            void SetPosition(const Vec2& position) const;
            [[nodiscard]] f32 GetRotation() const;
            void SetRotation(const f32 rotation) const;

            [[nodiscard]] bool HasFixedRotation() const;
            void SetFixedRotation(const bool hasFixedRotation) const;

            [[nodiscard]] Vec2 GetLinearVelocity() const;
            void SetLinearVelocity(const Vec2& linearVelocity) const;
            [[nodiscard]] f32 GetAngularVelocity() const;
            void SetAngularVelocity(const f32 angularVelocity) const;

            [[nodiscard]] f32 GetLinearDamping() const;
            void SetLinearDamping(const f32 linearDamping) const;
            [[nodiscard]] f32 GetAngularDamping() const;
            void SetAngularDamping(const f32 angularDamping) const;

            [[nodiscard]] f32 GetGravityScale() const;
            void SetGravityScale(const f32 gravityScale) const;

            [[nodiscard]] f32 GetMass() const;
            [[nodiscard]] f32 GetInertia() const;
            [[nodiscard]] MassData GetMassData() const;
            void SetMassData(const MassData& massData) const;
            void ResetMassData() const;

            [[nodiscard]] inline ObserverPtr<b2Body> GetRawHandle() const noexcept { return m_handle; }
            [[nodiscard]] inline ObserverPtr<const class World> GetOwningWorld() const noexcept { return m_owningWorld; }
        };
    }
}

#endif