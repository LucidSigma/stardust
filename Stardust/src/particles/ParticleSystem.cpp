#include "stardust/particles/ParticleSystem.h"

#include "stardust/graphics/renderer/FlipType.h"
#include "stardust/math/Math.h"
#include "stardust/scene/components/Components.h"
#include "stardust/utility/random/Random.h"

namespace stardust
{
	ParticleSystem::ParticleSystem()
	{
		m_particlePool.resize(s_ParticleCount);
	}

	void ParticleSystem::Update(const f32 deltaTime)
	{
		Vector<usize> particlesToRemove{ };

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
			particle->velocity *= 1.0f + (particle->acceleration * deltaTime);
			particle->angularVelocity *= 1.0f + (particle->angularAcceleration * deltaTime);

			if (particle->isAffectedByGravity)
			{
				particle->velocity += m_gravity * deltaTime;
			}

			particle->position += particle->velocity * deltaTime;
			particle->rotation += particle->angularVelocity * deltaTime;
			particle->size *= 1.0f + (particle->sizeUpdateMultipler * deltaTime);

			particle->currentColour = Colour(
				glm::lerp(
					Vec4(particle->endColour),
					Vec4(particle->startColour),
					particle->lifetimeRemaining / particle->lifetime
				)
			);
		}

		for (const auto particleID : particlesToRemove)
		{
			m_activeParticles.erase(particleID);
		}
	}

	void ParticleSystem::RenderInWorld(Renderer& renderer, const Camera2D& camera) const
	{
		for (const auto& [particleID, particle] : m_activeParticles)
		{
			if (!particle->isActive)
			{
				continue;
			}

			if (particle->texture == nullptr)
			{
				renderer.DrawWorldRect(
					components::Transform(particle->position, particle->rotation, NullOpt, particle->size),
					particle->currentColour,
					camera
				);
			}
			else
			{
				renderer.DrawWorldRect(
					components::Transform(particle->position, particle->rotation, NullOpt, particle->size),
					components::Sprite(*particle->texture, particle->textureArea, particle->currentColour),
					camera
				);
			}
		}
	}

	void ParticleSystem::RenderOnScreen(Renderer& renderer) const
	{
		for (const auto& [particleID, particle] : m_activeParticles)
		{
			if (!particle->isActive)
			{
				continue;
			}

			if (particle->texture == nullptr)
			{
				renderer.DrawScreenRect(
					components::ScreenTransform(IVec2(particle->position), IVec2(particle->size), FlipType::None, particle->rotation, NullOpt),
					particle->currentColour
				);
			}
			else
			{
				renderer.DrawScreenRect(
					components::ScreenTransform(IVec2(particle->position), IVec2(particle->size), FlipType::None, particle->rotation, NullOpt),
					components::Sprite(*particle->texture, particle->textureArea, particle->currentColour)
				);
			}
		}
	}

	void ParticleSystem::Emit(const ParticleData& particleData)
	{
		Particle& particle = m_particlePool[m_particlePoolIndex];

		particle.position = particleData.initialPosition;
		particle.rotation = particleData.initialRotation;

		particle.velocity.x = Random::GenerateFloat(particleData.minVelocity.x, particleData.maxVelocity.x);
		particle.velocity.y = Random::GenerateFloat(particleData.minVelocity.y, particleData.maxVelocity.y);
		particle.acceleration = particleData.acceleration;

		particle.angularVelocity = Random::GenerateFloat(particleData.minAngularVelocity, particleData.maxAngularVelocity);
		particle.angularAcceleration = particleData.angularAcceleration;

		particle.isAffectedByGravity = particleData.isAffectedByGravity;

		particle.size.x = Random::GenerateFloat(particleData.minSize.x, particleData.maxSize.x);
		particle.size.y = particleData.keepAsSquare ? particle.size.x : Random::GenerateFloat(particleData.minSize.y, particleData.maxSize.y);
		particle.sizeUpdateMultipler = particleData.sizeUpdateMultipler;

		if (particleData.shiftToCentre)
		{
			particle.position -= particle.size / 2.0f;
		}

		particle.currentColour = particleData.startColour;
		particle.startColour = particleData.startColour;
		particle.endColour = particleData.endColour;
		particle.texture = particleData.texture;
		particle.textureArea = particleData.textureArea;

		particle.lifetime = Random::GenerateFloat(particleData.minLifetime, particleData.maxLifetime);
		particle.lifetimeRemaining = particle.lifetime;

		particle.isActive = true;

		m_activeParticles.insert({ m_particlePoolIndex, &m_particlePool[m_particlePoolIndex] });

		--m_particlePoolIndex;
		m_particlePoolIndex %= s_ParticleCount;
	}

	void ParticleSystem::KillAllParticles()
	{
		m_activeParticles.clear();
	}

	void ParticleSystem::RepositionAllActiveParticles(const Vec2& relativePosition)
	{
		for (auto& [particleID, particle] : m_activeParticles)
		{
			particle->position += relativePosition;
		}
	}

	void ParticleSystem::ResizeAllActiveParticles(const f32 relativeScale)
	{
		for (auto& [particleID, particle] : m_activeParticles)
		{
			particle->size *= relativeScale;
		}
	}
}