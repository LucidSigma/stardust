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

                f32 maxBoidSpeed;
            };

            class Boid
            {
            private:
                Vec2 m_position;
                Vec2 m_velocity;
                Vec2 m_acceleration = Vec2Zero;

            public:
                Boid(const Vec2& position, const Vec2& velocity);
                ~Boid() noexcept = default;

                void Update(const f32 deltaTime);

                [[nodiscard]] const Vec2& GetPosition() const noexcept { return m_position; }
                [[nodiscard]] const Vec2& GetVelocity() const noexcept { return m_velocity; }
                [[nodiscard]] const Vec2& GetAcceleration() const noexcept { return m_acceleration; }
            };

        private:
            Vector<Boid> m_boids{ };

        public:
            BoidFlock() = default;
            BoidFlock(const CreateInfo& createInfo);
            ~BoidFlock() noexcept = default;

            void Initialise(const CreateInfo& createInfo);

            void Update(const f32 deltaTime);

            [[nodiscard]] inline const Vector<Boid>& GetBoids() const noexcept { return m_boids; }
        };
    }
}

#endif