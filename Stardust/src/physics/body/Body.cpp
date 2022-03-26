#include "stardust/physics/body/Body.h"

#include <utility>

#include "stardust/physics/world/World.h"

namespace stardust
{
    namespace physics
    {
        Body::Body(const World& world, const Body::CreateInfo& createInfo, const EntityHandle associatedEntityHandle)
        {
            Initialise(world, createInfo, associatedEntityHandle);
        }

        Body::Body(Body&& other) noexcept
            : m_handle(nullptr), m_owningWorld(nullptr), m_colliders({ }), m_associatedEntityHandle(NullEntityHandle)
        {
            std::swap(m_handle, other.m_handle);
            std::swap(m_owningWorld, other.m_owningWorld);
            std::swap(m_colliders, other.m_colliders);
            std::swap(m_associatedEntityHandle, other.m_associatedEntityHandle);
        }

        auto Body::operator =(Body&& other) noexcept -> Body&
        {
            m_handle = std::exchange(other.m_handle, nullptr);
            m_owningWorld = std::exchange(other.m_owningWorld, nullptr);
            m_colliders = std::exchange(other.m_colliders, { });
            m_associatedEntityHandle = std::exchange(other.m_associatedEntityHandle, NullEntityHandle);

            return *this;
        }

        auto Body::ApplyForce(const Vector2 force, const Vector2 point, const bool wakeUp) const -> void
        {
            m_handle->ApplyForce(b2Vec2{ force.x, force.y }, b2Vec2{ point.x, point.y }, wakeUp);
        }

        auto Body::ApplyForceToCentre(const Vector2 force, const bool wakeUp) const -> void
        {
            m_handle->ApplyForceToCenter(b2Vec2{ force.x, force.y }, wakeUp);
        }

        auto Body::ApplyTorque(const f32 torque, const bool wakeUp) const -> void
        {
            m_handle->ApplyTorque(torque, wakeUp);
        }

        auto Body::ApplyLinearImpulse(const Vector2 impulse, const Vector2 point, const bool wakeUp) const -> void
        {
            m_handle->ApplyLinearImpulse(b2Vec2{ impulse.x, impulse.y }, b2Vec2{ point.x, point.y }, wakeUp);
        }

        auto Body::ApplyLinearImpulseToCentre(const Vector2 impulse, const bool wakeUp) const -> void
        {
            m_handle->ApplyLinearImpulseToCenter(b2Vec2{ impulse.x, impulse.y }, wakeUp);
        }

        auto Body::ApplyAngularImpulse(const f32 impulse, const bool wakeUp) const -> void
        {
            m_handle->ApplyAngularImpulse(impulse, wakeUp);
        }

        auto Body::Move(const Vector2 positionOffset) const -> void
        {
            SetPosition(GetPosition() + positionOffset);
        }

        auto Body::Rotate(const f32 angleOffset) const -> void
        {
            SetRotation(GetRotation() + angleOffset);
        }

        auto Body::AddCollider(const Collider::CreateInfo& colliderInfo) -> Collider
        {
            return *m_colliders.emplace(*this, colliderInfo).first;
        }

        auto Body::RemoveCollider(const Collider& collider) -> void
        {
            m_colliders.erase(collider);
            m_handle->DestroyFixture(collider.GetRawHandle());
        }

        [[nodiscard]] auto Body::GetWorldCentre() const -> Vector2
        {
            const b2Vec2 worldCentre = m_handle->GetWorldCenter();

            return Vector2{ worldCentre.x, worldCentre.y };
        }

        [[nodiscard]] auto Body::GetLocalCenter() const -> Vector2
        {
            const b2Vec2 localCentre = m_handle->GetLocalCenter();

            return Vector2{ localCentre.x, localCentre.y };
        }

        [[nodiscard]] auto Body::GetWorldPoint(const Vector2 localPoint) const -> Vector2
        {
            const b2Vec2 worldPoint = m_handle->GetWorldPoint(b2Vec2{ localPoint.x, localPoint.y });

            return Vector2{ worldPoint.x, worldPoint.y };
        }

        [[nodiscard]] auto Body::GetWorldVector(const Vector2 localVector) const -> Vector2
        {
            const b2Vec2 worldVector = m_handle->GetWorldVector(b2Vec2{ localVector.x, localVector.y });

            return Vector2{ worldVector.x, worldVector.y };
        }

        [[nodiscard]] auto Body::GetLocalPoint(const Vector2 worldPoint) const -> Vector2
        {
            const b2Vec2 localPoint = m_handle->GetLocalPoint(b2Vec2{ worldPoint.x, worldPoint.y });

            return Vector2{ localPoint.x, localPoint.y };
        }

        [[nodiscard]] auto Body::GetLocalVector(const Vector2 worldVector) const -> Vector2
        {
            const b2Vec2 localVector = m_handle->GetLocalVector(b2Vec2{ worldVector.x, worldVector.y });

            return Vector2{ localVector.x, localVector.y };
        }

        [[nodiscard]] auto Body::GetLinearVelocityFromWorldPoint(const Vector2 worldPoint) const -> Vector2
        {
            const b2Vec2 linearVelocity = m_handle->GetLinearVelocityFromWorldPoint(b2Vec2{ worldPoint.x, worldPoint.y });

            return Vector2{ linearVelocity.x, linearVelocity.y };
        }

        [[nodiscard]] auto Body::GetLinearVelocityFromLocalPoint(const Vector2 localPoint) const -> Vector2
        {
            const b2Vec2 linearVelocity = m_handle->GetLinearVelocityFromLocalPoint(b2Vec2{ localPoint.x, localPoint.y });

            return Vector2{ linearVelocity.x, linearVelocity.y };
        }

        [[nodiscard]] auto Body::IsEnabled() const -> bool
        {
            return m_handle->IsEnabled();
        }

        auto Body::SetEnabled(const bool isEnabled) const -> void
        {
            m_handle->SetEnabled(isEnabled);
        }

        [[nodiscard]] auto Body::IsAwake() const -> bool
        {
            return m_handle->IsAwake();
        }

        [[nodiscard]] auto Body::CanSleep() const -> bool
        {
            return m_handle->IsSleepingAllowed();
        }

        auto Body::SetAllowSleeping(const bool canSleep) const -> void
        {
            m_handle->SetSleepingAllowed(canSleep);
        }

        [[nodiscard]] auto Body::GetType() const -> Type
        {
            return static_cast<Type>(m_handle->GetType());
        }

        auto Body::SetType(const Type type) const -> void
        {
            m_handle->SetType(static_cast<b2BodyType>(type));
        }

        [[nodiscard]] auto Body::IsBullet() const -> bool
        {
            return m_handle->IsBullet();
        }

        auto Body::SetBullet(const bool isBullet) const -> void
        {
            m_handle->SetBullet(isBullet);
        }

        [[nodiscard]] auto Body::GetPosition() const -> Vector2
        {
            const b2Vec2 position = m_handle->GetPosition();

            return Vector2{ position.x, position.y };
        }

        auto Body::SetPosition(const Vector2 position) const -> void
        {
            m_handle->SetTransform(b2Vec2{ position.x, position.y }, GetRotation());
        }

        [[nodiscard]] auto Body::GetRotation() const -> f32
        {
            return -glm::degrees(m_handle->GetAngle());
        }

        auto Body::SetRotation(const f32 rotation) const -> void
        {
            m_handle->SetTransform(m_handle->GetPosition(), -glm::radians(rotation));
        }

        [[nodiscard]] auto Body::HasFixedRotation() const -> bool
        {
            return m_handle->IsFixedRotation();
        }

        auto Body::SetFixedRotation(const bool hasFixedRotation) const -> void
        {
            m_handle->SetFixedRotation(hasFixedRotation);
        }

        [[nodiscard]] auto Body::GetLinearVelocity() const -> Vector2
        {
            const b2Vec2 linearVelocity = m_handle->GetLinearVelocity();

            return Vector2{ linearVelocity.x, linearVelocity.y };
        }

        auto Body::SetLinearVelocity(const Vector2 linearVelocity) const -> void
        {
            m_handle->SetLinearVelocity(b2Vec2{ linearVelocity.x, linearVelocity.y });
        }

        [[nodiscard]] auto Body::GetAngularVelocity() const -> f32
        {
            return m_handle->GetAngularVelocity();
        }

        auto Body::SetAngularVelocity(const f32 angularVelocity) const -> void
        {
            m_handle->SetAngularVelocity(angularVelocity);
        }

        [[nodiscard]] auto Body::GetLinearDamping() const -> f32
        {
            return m_handle->GetLinearDamping();
        }

        auto Body::SetLinearDamping(const f32 linearDamping) const -> void
        {
            m_handle->SetLinearDamping(linearDamping);
        }

        [[nodiscard]] auto Body::GetAngularDamping() const -> f32
        {
            return m_handle->GetAngularDamping();
        }

        auto Body::SetAngularDamping(const f32 angularDamping) const -> void
        {
            m_handle->SetAngularDamping(angularDamping);
        }

        [[nodiscard]] auto Body::GetGravityScale() const -> f32
        {
            return m_handle->GetGravityScale();
        }

        auto Body::SetGravityScale(const f32 gravityScale) const -> void
        {
            m_handle->SetGravityScale(gravityScale);
        }

        [[nodiscard]] auto Body::GetMass() const -> f32
        {
            return m_handle->GetMass();
        }

        [[nodiscard]] auto Body::GetInertia() const -> f32
        {
            return m_handle->GetInertia();
        }

        [[nodiscard]] auto Body::GetMassData() const -> MassData
        {
            b2MassData massData{ };
            m_handle->GetMassData(&massData);

            return MassData{
                .mass = massData.mass,
                .centre = Vector2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }

        auto Body::SetMassData(const MassData& massData) const -> void
        {
            const b2MassData convertedMassData{
                .mass = massData.mass,
                .center = b2Vec2{ massData.centre.x, massData.centre.y },
                .I = massData.momentOfInertia,
            };

            m_handle->SetMassData(&convertedMassData);
        }

        auto Body::ResetMassData() const -> void
        {
            m_handle->ResetMassData();
        }

        auto Body::Initialise(const World& world, const CreateInfo& createInfo, const EntityHandle associatedEntityHandle) -> void
        {
            b2BodyDef bodyDef{ };
            bodyDef.type = static_cast<b2BodyType>(createInfo.type);
            bodyDef.position = b2Vec2{ createInfo.position.x, createInfo.position.y };
            bodyDef.angle = createInfo.angle;
            bodyDef.linearVelocity = b2Vec2{ createInfo.linearVelocity.x, createInfo.linearVelocity.y };
            bodyDef.angularVelocity = createInfo.angularVelocity;
            bodyDef.linearDamping = createInfo.linearDamping;
            bodyDef.angularDamping = createInfo.angularDamping;
            bodyDef.allowSleep = createInfo.allowSleep;
            bodyDef.awake = createInfo.isAwake;
            bodyDef.fixedRotation = createInfo.hasFixedRotation;
            bodyDef.bullet = createInfo.isBullet;
            bodyDef.enabled = createInfo.isEnabled;
            bodyDef.gravityScale = createInfo.gravityScale;
            bodyDef.userData = b2BodyUserData{ };

            m_handle = world.GetRawHandle()->CreateBody(&bodyDef);
            m_owningWorld = &world;
            m_associatedEntityHandle = associatedEntityHandle;

            if (m_handle != nullptr)
            {
                for (const auto& fixtureInfo : createInfo.colliderInfos)
                {
                    AddCollider(fixtureInfo);
                }
            }
        }
    }
}
