#pragma once
#ifndef STARDUST_PARTICLE_CONTROLLER_COMPONENT_H
#define STARDUST_PARTICLE_CONTROLLER_COMPONENT_H

#include "stardust/data/Pointers.h"
#include "stardust/particles/ParticleSystem.h"

namespace stardust
{
	namespace components
	{
		struct ParticleController
		{
			ObserverPtr<ParticleSystem> particleSystem;

			ParticleController()
				: particleSystem(nullptr)
			{ }

			explicit ParticleController(ParticleSystem& particleSystem)
				: particleSystem(&particleSystem)
			{ }

			~ParticleController() noexcept = default;
		};
	}
}

#endif