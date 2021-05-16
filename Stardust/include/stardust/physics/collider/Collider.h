#pragma once
#ifndef STARDUST_COLLIDER_H
#define STARDUST_COLLIDER_H

#include <functional>

#include <box2d/box2d.h>

#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
    namespace physics
    {
        class Collider
        {
        public:
            struct Filter
            {
                CollisionLayer layers = 0x0001;
                CollisionLayer collidesWith = AllLayers;

                i16 groupIndex = 0;
            };

            struct CreateInfo
            {
                ObserverPtr<const Shape> shape = nullptr;

                f32 friction = 0.2;
                f32 restitution = 0.0f;
                f32 restitutionThreshold = 1.0f * b2_lengthUnitsPerMeter;
                f32 density = 0.0f;

                bool isSensor = false;

                Filter filter{ };
            };

        private:
            ObserverPtr<b2Fixture> m_handle = nullptr;
            ObserverPtr<const class Body> m_owningBody = nullptr;

        public:
            Collider() = default;
            Collider(const class Body& body, const CreateInfo& createInfo);
            Collider(const ObserverPtr<b2Fixture> fixtureHandle, const class World& world);
            ~Collider() noexcept = default;

            inline bool IsValid() const noexcept { return m_handle != nullptr; }

            [[nodiscard]] ShapeType GetShapeType() const;

            [[nodiscard]] ObserverPtr<b2Shape> GetShape();
            [[nodiscard]] ObserverPtr<const b2Shape> GetShape() const;

            [[nodiscard]] f32 GetFriction() const;
            void SetFriction(const f32 friction) const;

            [[nodiscard]] f32 GetRestitution() const;
            void SetRestitution(const f32 restitution) const;

            [[nodiscard]] f32 GetRestitutionThreshold() const;
            void SetRestitutionThreshold(const f32 restitutionThreshold) const;

            [[nodiscard]] f32 GetDensity() const;
            void SetDensity(const f32 density) const;

            [[nodiscard]] bool IsSensor() const;
            void SetSensor(const bool isSensor) const;

            [[nodiscard]] Filter GetFilterData() const;
            void SetFilterData(const Filter& filterData) const;
            void Refilter() const;

            [[nodiscard]] AABB GetAABB() const;
            [[nodiscard]] MassData GetMassData() const;

            inline ObserverPtr<const class Body> GetOwningBody() const noexcept { return m_owningBody; }
            inline ObserverPtr<b2Fixture> GetRawHandle() const noexcept { return m_handle; }

            inline bool operator ==(const Collider& other) const noexcept = default;
            inline bool operator !=(const Collider& other) const noexcept = default;
        };
    }
}

namespace std
{
    template <>
    struct hash<stardust::physics::Collider>
    {
        [[nodiscard]] inline std::size_t operator ()(const stardust::physics::Collider& collider) const noexcept
        {
            return std::hash<stardust::ObserverPtr<b2Fixture>>()(collider.GetRawHandle());
        }
    };
}

#endif