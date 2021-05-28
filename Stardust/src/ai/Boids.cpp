#include "stardust/ai/Boids.h"

#include "stardust/utility/random/Random.h"

namespace stardust
{
    namespace ai
    {
        BoidFlock::Boid::Boid(const CreateInfo& createInfo)
            : m_position(createInfo.position), m_velocity(createInfo.velocity), m_maxSpeed(glm::length(createInfo.velocity)), m_maxSteeringForce(createInfo.maxSteeringForce), m_perceptionRadius(createInfo.perceptionRadius)
        { }

        void BoidFlock::Boid::Update(const f32 deltaTime, const BoidFlock& flock)
        {
            m_acceleration = GetFlockForce(flock);

            m_velocity += m_acceleration * deltaTime;
            m_velocity = LimitMagnitude(m_velocity, m_maxSpeed);

            m_position += m_velocity * deltaTime;
        }

        [[nodiscard]] Vec2 BoidFlock::Boid::GetFlockForce(const BoidFlock& flock) const
        {
            Vec2 separationForce = Vec2Zero;
            Vec2 alignmentForce = Vec2Zero;
            Vec2 cohesionForce = Vec2Zero;

            u32 boidsWithinRange = 0u;

            for (const auto& boid : flock.GetBoids())
            {
                if (&boid != this) [[likely]]
                {
                    if (const f32 distance = glm::distance(GetPosition(), boid.GetPosition());
                        distance <= m_perceptionRadius)
                    {
                        Vec2 difference = m_position - boid.GetPosition();
                        difference /= distance * distance;

                        separationForce += difference;
                        alignmentForce += boid.GetVelocity();
                        cohesionForce += boid.GetPosition();

                        ++boidsWithinRange;
                    }
                }
            }

            if (boidsWithinRange == 0u)
            {
                return Vec2Zero;
            }

            separationForce /= static_cast<f32>(boidsWithinRange);
            separationForce = SetMagnitude(separationForce, m_maxSpeed);
            separationForce -= m_velocity;
            separationForce *= flock.GetSeparationFactor();

            alignmentForce /= static_cast<f32>(boidsWithinRange);
            alignmentForce = SetMagnitude(alignmentForce, m_maxSpeed);
            alignmentForce -= m_velocity;
            alignmentForce *= flock.GetAlignmentFactor();

            cohesionForce /= static_cast<f32>(boidsWithinRange);
            cohesionForce -= m_position;
            cohesionForce = SetMagnitude(cohesionForce, m_maxSpeed);
            cohesionForce -= m_velocity;
            cohesionForce *= flock.GetCohesionFactor();

            return LimitMagnitude(separationForce + alignmentForce + cohesionForce, m_maxSteeringForce);
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
                boid.Update(deltaTime, *this);
            }
        }
    }
}