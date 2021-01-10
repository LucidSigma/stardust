#pragma once
#ifndef STARDUST_PARTICLE_GENERATOR_COMPONENT_H
#define STARDUST_PARTICLE_GENERATOR_COMPONENT_H

#include "stardust/data/Pointers.h"
#include "stardust/particles/ParticleSystem.h"

namespace stardust
{
	namespace components
	{
		struct ParticleGenerator
		{
			ObserverPtr<ParticleSystem> particleSystem;

			ParticleGenerator()
				: particleSystem(nullptr)
			{ }

			explicit ParticleGenerator(ParticleSystem& particleSystem)
				: particleSystem(&particleSystem)
			{ }

			~ParticleGenerator() noexcept = default;
		};
	}
}

#endif