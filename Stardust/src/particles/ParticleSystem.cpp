#include "stardust/particles/ParticleSystem.h"

#include "stardust/math/random/Random.h"
#include "stardust/math/Math.h"

namespace stardust
{
    ParticleSystem::ParticleSystem()
    {
        m_particlePool.resize(s_MaxParticleCount);
    }

    auto ParticleSystem::Update(const f32 deltaTime) -> void
    {
        List<usize> particlesToRemove{ };

        for (auto& [particleID, particle] : m_activeParticles)
        {
            if (particle->lifetimeRemaining <= 0.0f)
            {
                particle->isActive = false;
            }

            if (!particle->isActive)
            {
                particlesToRemove.push_back(particleID);

                continue;
            }

            particle->lifetimeRemaining -= deltaTime;
            particle->velocity += particle->acceleration * deltaTime;
            particle->angularVelocity += particle->angularAcceleration * deltaTime;

            if (particle->isAffectedByGravity)
            {
                particle->velocity.y += m_gravity * deltaTime;
            }

            particle->position += particle->velocity * deltaTime;

            if (particle->isAffectedByWind)
            {
                particle->position.x += m_wind * deltaTime;
            }

            particle->rotation += particle->angularVelocity * deltaTime;
            particle->size *= particle->sizeUpdateMultiplier * deltaTime;

            particle->currentColour = Colour(
                glm::lerp(
                    Vector4(particle->endColour),
                    Vector4(particle->startColour),
                    particle->colourEasingFunction(particle->lifetimeRemaining / particle->totalLifetime)
                )
            );

            if (particle->callback.has_value())
            {
                if (particle->callback.value()(*particle, particle->callbackUserData) == ParticleCallbackResult::Kill)
                {
                    particlesToRemove.push_back(particleID);
                }
            }
        }

        for (const auto particleID : particlesToRemove)
        {
            m_activeParticles.erase(particleID);
        }
    }

    [[nodiscard]] auto ParticleSystem::GenerateParticleComponents() const -> Generator<const Pair<components::Transform, components::Sprite>>
    {
        for (const auto [particleID, particle] : m_activeParticles)
        {
            if (!particle->isActive) [[unlikely]]
            {
                continue;
            }

            co_yield {
                components::Transform{
                    .translation = particle->position,
                    .scale = particle->size,
                    .reflection = particle->reflection,
                    .rotation = particle->rotation,
                    .pivot = particle->pivot,
                    .shear = particle->shear,
                },
                components::Sprite{
                    .texture = particle->texture,
                    .subTextureArea = particle->textureArea,
                    .colourMod = particle->currentColour,
                },
            };
        }
    }

    auto ParticleSystem::Emit(const ParticleData& particleData) -> void
    {
        Particle& particle = m_particlePool[m_particlePoolIndex];

        particle.position = particleData.initialPosition;
        particle.rotation = particleData.initialRotation;

        particle.velocity.x = Random::GenerateFloat(particleData.initialVelocityRange.first.x, particleData.initialVelocityRange.second.x);
        particle.velocity.y = Random::GenerateFloat(particleData.initialVelocityRange.first.y, particleData.initialVelocityRange.second.y);
        particle.acceleration = Random::GenerateFloat(particleData.initialAccelerationRange.first, particleData.initialAccelerationRange.second);

        particle.angularVelocity = Random::GenerateFloat(particleData.initialAngularVelocityRange.first, particleData.initialAngularVelocityRange.second);
        particle.angularAcceleration = Random::GenerateFloat(particleData.initialAngularAccelerationRange.first, particleData.initialAngularAccelerationRange.second);
        particle.pivot = particleData.pivot;

        particle.isAffectedByGravity = particleData.isAffectedByGravity;
        particle.isAffectedByWind = particleData.isAffectedByWind;

        particle.size.x = Random::GenerateFloat(particleData.initialSizeRange.first.x, particleData.initialSizeRange.second.x);
        particle.size.y = particleData.keepAsSquare ? particle.size.x : Random::GenerateFloat(particleData.initialSizeRange.first.y, particleData.initialSizeRange.second.y);
        particle.sizeUpdateMultiplier = particleData.sizeUpdateMultiplier;

        if (particleData.initialShearRange.has_value())
        {
            particle.shear = Vector2Zero;
            particle.shear.value().x = Random::GenerateFloat(particleData.initialShearRange.value().first.x, particleData.initialShearRange.value().second.x);
            particle.shear.value().y = Random::GenerateFloat(particleData.initialShearRange.value().first.y, particleData.initialShearRange.value().second.y);
        }
        else
        {
            particle.shear = None;
        }

        particle.currentColour = particleData.startColour;
        particle.startColour = particleData.startColour;
        particle.endColour = particleData.endColour;

        particle.texture = particleData.texture;
        particle.textureArea = particleData.textureArea;
        particle.reflection = particleData.reflection;

        particle.colourEasingFunction = particleData.colourEasingFunction;

        particle.totalLifetime = Random::GenerateFloat(particleData.initialLifetimeRange.first, particleData.initialLifetimeRange.second);
        particle.lifetimeRemaining = particle.totalLifetime;

        particle.callback = particleData.callback;
        particle.callbackUserData = particleData.callbackUserData;

        particle.isActive = true;

        m_activeParticles.insert({ m_particlePoolIndex, &m_particlePool[m_particlePoolIndex] });

        ++m_particlePoolIndex;
        m_particlePoolIndex %= s_MaxParticleCount;
    }

    auto ParticleSystem::KillAllParticles() -> void
    {
        m_activeParticles.clear();
    }

    auto ParticleSystem::RepositionAllActiveParticles(const Vector2 relativePosition) -> void
    {
        for (auto& [particleID, particle] : m_activeParticles)
        {
            particle->position += relativePosition;
        }
    }

    auto ParticleSystem::ResizeAllActiveParticles(const f32 relativeScale) -> void
    {
        for (auto& [particleID, particle] : m_activeParticles)
        {
            particle->size *= relativeScale;
        }
    }
}
