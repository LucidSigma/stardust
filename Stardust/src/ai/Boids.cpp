#include "stardust/ai/Boids.h"

#include "stardust/utility/random/Random.h"

namespace stardust
{
    namespace ai
    {
        BoidFlock::Boid::Boid(const CreateInfo& createInfo)
            : m_position(createInfo.position), m_velocity(createInfo.velocity), m_maxSpeed(glm::length(createInfo.velocity)), m_maxSteeringForce(createInfo.maxSteeringForce), m_perceptionRadius(createInfo.perceptionRadius)
        { }

        void BoidFlock::Boid::Update(const f32 deltaTime, const Vector<Boid>& boids)
        {
            const Vec2 separationForce = Separate(boids);
            const Vec2 alignForce = Align(boids);
            const Vec2 cohesionForce = Cohere(boids);

            m_acceleration += LimitMagnitude(separationForce + alignForce + cohesionForce, m_maxSteeringForce) * deltaTime;

            m_velocity += m_acceleration * deltaTime;
            m_velocity = LimitMagnitude(m_velocity, m_maxSpeed);

            m_position += m_velocity * deltaTime;
        }

        [[nodiscard]] Vec2 BoidFlock::Boid::Separate(const Vector<Boid>& boids) const
        {
            Vec2 steeringVelocity = Vec2Zero;
            u32 boidsWithinRange = 0u;

            for (const auto& boid : boids)
            {
                if (&boid != this) [[likely]]
                {
                    if (const f32 distance = glm::distance(GetPosition(), boid.GetPosition());
                        distance <= m_perceptionRadius)
                    {
                        Vec2 difference = m_position - boid.GetPosition();
                        difference /= distance * distance;

                        steeringVelocity += difference;
                        ++boidsWithinRange;
                    }
                }
            }

            if (boidsWithinRange == 0u)
            {
                return Vec2Zero;
            }

            steeringVelocity /= static_cast<f32>(boidsWithinRange);
            steeringVelocity = SetMagnitude(steeringVelocity, m_maxSpeed);
            steeringVelocity -= m_velocity;

            return steeringVelocity;
        }

        [[nodiscard]] Vec2 BoidFlock::Boid::Align(const Vector<Boid>& boids) const
        {
            Vec2 steeringVelocity = Vec2Zero;
            u32 boidsWithinRange = 0u;

            for (const auto& boid : boids)
            {
                if (&boid != this) [[likely]]
                {
                    if (const f32 distance = glm::distance(GetPosition(), boid.GetPosition());
                        distance <= m_perceptionRadius)
                    {
                        steeringVelocity += boid.GetVelocity();
                        ++boidsWithinRange;
                    }
                }
            }

            if (boidsWithinRange == 0u)
            {
                return Vec2Zero;
            }

            steeringVelocity /= static_cast<f32>(boidsWithinRange);
            steeringVelocity = SetMagnitude(steeringVelocity, m_maxSpeed);
            steeringVelocity -= m_velocity;

            return steeringVelocity;
        }

        [[nodiscard]] Vec2 BoidFlock::Boid::Cohere(const Vector<Boid>& boids) const
        {
            Vec2 steeringVelocity = Vec2Zero;
            u32 boidsWithinRange = 0u;

            for (const auto& boid : boids)
            {
                if (&boid != this) [[likely]]
                {
                    if (const f32 distance = glm::distance(GetPosition(), boid.GetPosition());
                        distance <= m_perceptionRadius)
                    {
                        steeringVelocity += boid.GetPosition();
                        ++boidsWithinRange;
                    }
                }
            }

            if (boidsWithinRange == 0u)
            {
                return Vec2Zero;
            }

            steeringVelocity /= static_cast<f32>(boidsWithinRange);
            steeringVelocity -= m_position;

            steeringVelocity = SetMagnitude(steeringVelocity, m_maxSpeed);
            steeringVelocity -= m_velocity;

            return steeringVelocity;
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
                    Boid::CreateInfo{
                        .position = RandomVec2(-createInfo.spawnRadius, createInfo.spawnRadius),
                        .velocity = RandomUnitVec2() * Random::GenerateFloat(createInfo.minBoidSpeed, createInfo.maxBoidSpeed),
                        .maxSteeringForce = Random::GenerateFloat(createInfo.minBoidSteeringForce, createInfo.maxBoidSteeringForce),
                        .perceptionRadius = Random::GenerateFloat(createInfo.minBoidPerceptionRadius, createInfo.maxBoidPerceptionRadius),
                    }
                );
            }
        }

        void BoidFlock::Update(const f32 deltaTime)
        {
            for (auto& boid : m_boids)
            {
                boid.Update(deltaTime, m_boids);
            }
        }
    }
}