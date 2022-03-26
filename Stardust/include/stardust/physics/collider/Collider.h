#pragma once
#ifndef STARDUST_COLLIDER_H
#define STARDUST_COLLIDER_H

#include <functional>

#include <box2d/box2d.h>

#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        class Collider final
        {
        public:
            using GroupIndex = i32;

            struct Filter final
            {
                CollisionLayer layers = 0x0001;
                CollisionLayer collidesWith = AllLayers;

                GroupIndex groupIndex = 0;
            };

            struct CreateInfo final
            {
                ObserverPointer<const Shape> shape = nullptr;

                f32 friction = 0.0f;
                f32 restitution = 0.0f;
                f32 restitutionThreshold = 1.0f * b2_lengthUnitsPerMeter;
                f32 density = 0.0f;

                bool isSensor = false;

                Filter filter{ };
            };

        private:
            ObserverPointer<b2Fixture> m_handle = nullptr;
            ObserverPointer<const class Body> m_owningBody = nullptr;

        public:
            Collider() = default;
            Collider(const class Body& body, const CreateInfo& createInfo);
            Collider(const ObserverPointer<b2Fixture> fixtureHandle, const class World& world);

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr; }

            [[nodiscard]] auto GetShapeType() const -> ShapeType;

            [[nodiscard]] auto GetShape() -> ObserverPointer<b2Shape>;
            [[nodiscard]] auto GetShape() const -> ObserverPointer<const b2Shape>;

            [[nodiscard]] auto GetVertexCount() const -> u32;
            [[nodiscard]] auto GetVertices() const -> List<Vector2>;

            [[nodiscard]] auto GetFriction() const -> f32;
            auto SetFriction(const f32 friction) const -> void;

            [[nodiscard]] auto GetRestitution() const -> f32;
            auto SetRestitution(const f32 restitution) const -> void;

            [[nodiscard]] auto GetRestitutionThreshold() const -> f32;
            auto SetRestitutionThreshold(const f32 restitutionThreshold) const -> void;

            [[nodiscard]] auto GetDensity() const -> f32;
            auto SetDensity(const f32 density) const -> void;

            [[nodiscard]] auto IsSensor() const -> bool;
            auto SetSensor(const bool isSensor) const -> void;

            [[nodiscard]] auto GetFilterData() const -> Filter;
            auto SetFilterData(const Filter& filterData) const -> void;
            auto Refilter() const -> void;

            [[nodiscard]] auto GetAABB() const -> AABB;
            [[nodiscard]] auto GetMassData() const -> MassData;

            [[nodiscard]] inline auto GetOwningBody() const noexcept -> ObserverPointer<const class Body> { return m_owningBody; }
            [[nodiscard]] inline auto GetRawHandle() const noexcept -> ObserverPointer<b2Fixture> { return m_handle; }

            [[nodiscard]] inline auto operator ==(const Collider& other) const noexcept -> bool = default;
            [[nodiscard]] inline auto operator !=(const Collider& other) const noexcept -> bool = default;
        };
    }
}

namespace std
{
    template <>
    struct hash<stardust::physics::Collider>
    {
        [[nodiscard]] inline auto operator ()(const stardust::physics::Collider& collider) const noexcept -> stardust::usize
        {
            return std::hash<stardust::ObserverPointer<b2Fixture>>()(collider.GetRawHandle());
        }
    };
}

#endif
