#pragma once
#ifndef STARDUST_PARTICLE_SYSTEM_H
#define STARDUST_PARTICLE_SYSTEM_H

#include "stardust/camera/Camera2D.h"
#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/graphics/SortingLayer.h"

namespace stardust
{
	class ParticleSystem
	{
	public:
		struct ParticleData
		{
			Vec2 initialPosition{ 0.0f, 0.0f };
			f32 initialRotation = 0.0f;

			Vec2 minVelocity{ 0.0f, 0.0f };
			Vec2 maxVelocity{ 0.0f, 0.0f };
			f32 acceleration = 1.0f;

			f32 minAngularVelocity = 0.0f;
			f32 maxAngularVelocity = 0.0f;
			f32 angularAcceleration = 1.0f;

			bool isAffectedByGravity = false;

			Vec2 minSize{ 1.0f, 1.0f };
			Vec2 maxSize{ 1.0f, 1.0f };
			f32 sizeUpdateMultipler = 1.0f;
			bool keepAsSquare = false;
			bool shiftToCentre = false;

			Colour startColour = colours::White;
			Colour endColour = colours::White;
			ObserverPtr<const Texture> texture = nullptr;
			Optional<Pair<Vec2, Vec2>> textureArea = NullOpt;

			f32 minLifetime = 0.0f;
			f32 maxLifetime = 0.0f;
		};

	private:
		struct Particle
		{
			Vec2 position{ 0.0f, 0.0f };
			f32 rotation = 0.0f;

			Vec2 velocity{ 0.0f, 0.0f };
			f32 acceleration = 1.0f;

			f32 angularVelocity = 0.0f;
			f32 angularAcceleration = 1.0f;

			bool isAffectedByGravity = false;

			Vec2 size{ 0.0f, 0.0f };
			f32 sizeUpdateMultipler = 1.0f;

			Colour currentColour = colours::White;
			Colour startColour = colours::White;
			Colour endColour = colours::White;
			ObserverPtr<const Texture> texture = nullptr;
			Optional<Pair<Vec2, Vec2>> textureArea = NullOpt;

			f32 lifetime = 0.0f;
			f32 lifetimeRemaining = 0.0f;

			bool isActive = false;
		};

		static constexpr usize s_ParticleCount = 1'000u;

		Vector<Particle> m_particlePool;
		usize m_particlePoolIndex = s_ParticleCount - 1u;
		HashMap<usize, ObserverPtr<Particle>> m_activeParticles{ };

		ObserverPtr<const SortingLayer> m_sortingLayer = nullptr;

		Vec2 m_gravity = Vec2{ 0.0f, 0.0f };

	public:
		static constexpr usize GetMaxParticleCount() noexcept { return s_ParticleCount; }

		ParticleSystem() = default;
		ParticleSystem(const SortingLayer& sortingLayer);
		~ParticleSystem() noexcept = default;

		void Initialise(const SortingLayer& sortingLayer);

		void Update(const f32 deltaTime);
		void RenderInWorld(Renderer& renderer, const Camera2D& camera) const;
		void RenderOnScreen(Renderer& renderer) const;

		void Emit(const ParticleData& particleData);
		void KillAllParticles();

		void RepositionAllActiveParticles(const Vec2& relativePosition);
		void ResizeAllActiveParticles(const f32 relativeScale);

		inline usize GetActiveParticleCount() const noexcept { return m_activeParticles.size(); }

		inline const Vec2& GetGravity() const noexcept { return m_gravity; }
		inline void SetGravity(const Vec2& gravity) noexcept { m_gravity = gravity; }
	};
}

#endif