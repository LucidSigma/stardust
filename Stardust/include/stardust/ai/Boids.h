#pragma once
#ifndef STARDUST_BOIDS_H
#define STARDUST_BOIDS_H

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace ai
    {
        class BoidFlock
        {
        public:
            struct CreateInfo
            {
                usize initialBoidCount;
                f32 spawnRadius;

                f32 minBoidSpeed;
                f32 maxBoidSpeed;

                f32 minBoidSteeringForce;
                f32 maxBoidSteeringForce;

                f32 minBoidPerceptionRadius;
                f32 maxBoidPerceptionRadius;
            };

            class Boid
            {
            public:
                struct CreateInfo
                {
                    Vec2 position;
                    Vec2 velocity;

                    f32 maxSteeringForce;
                    f32 perceptionRadius;
                };

            private:
                Vec2 m_position;
                Vec2 m_velocity;
                Vec2 m_acceleration = Vec2Zero;

                f32 m_maxSpeed;
                f32 m_maxSteeringForce;
                f32 m_perceptionRadius;

            public:
                Boid(const CreateInfo& createInfo);
                ~Boid() noexcept = default;

                void Update(const f32 deltaTime, const Vector<Boid>& boids);

                [[nodiscard]] inline const Vec2& GetPosition() const noexcept { return m_position; }
                inline void SetPosition(const Vec2& position) noexcept { m_position = position; }

                [[nodiscard]] inline const Vec2& GetVelocity() const noexcept { return m_velocity; }
                inline void SetVelocity(const Vec2& velocity) noexcept { m_velocity = velocity; }

                [[nodiscard]] inline const Vec2& GetAcceleration() const noexcept { return m_acceleration; }
                inline void SetAcceleration(const Vec2& acceleration) noexcept { m_acceleration = acceleration; }

            private:
                [[nodiscard]] Vec2 GetFlockForce(const Vector<Boid>& boids) const;
            };

        private:
            Vector<Boid> m_boids{ };

        public:
            BoidFlock() = default;
            BoidFlock(const CreateInfo& createInfo);
            ~BoidFlock() noexcept = default;

            void Initialise(const CreateInfo& createInfo);

            void Update(const f32 deltaTime);

            [[nodiscard]] inline Vector<Boid>& GetBoids() noexcept { return m_boids; }
            [[nodiscard]] inline const Vector<Boid>& GetBoids() const noexcept { return m_boids; }
        };
    }
}

#endif