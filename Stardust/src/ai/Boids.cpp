#include "stardust/ai/Boids.h"

namespace stardust
{
    namespace ai
    {
        BoidFlock::Boid::Boid(const Vec2& position, const Vec2& velocity)
            : m_position(position), m_velocity(velocity)
        { }

        void BoidFlock::Boid::Update(const f32 deltaTime)
        {
            m_velocity += m_acceleration * deltaTime;
            m_position += m_velocity * deltaTime;
        }

        BoidFlock::BoidFlock(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        void BoidFlock::Initialise(const CreateInfo& createInfo)
        {
            m_boids.reserve(createInfo.initialBoidCount);

            for (usize i = 0u; i < createInfo.initialBoidCount; ++i)
            {
                m_boids.emplace_back(
                    RandomVec2(-createInfo.spawnRadius, createInfo.spawnRadius),
                    RandomUnitVec2() * createInfo.maxBoidSpeed
                );
            }
        }

        void BoidFlock::Update(const f32 deltaTime)
        {
            for (auto& boid : m_boids)
            {
                boid.Update(deltaTime);
            }
        }
    }
}