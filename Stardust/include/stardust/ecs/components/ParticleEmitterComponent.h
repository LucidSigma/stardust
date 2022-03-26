#pragma once
#ifndef STARDUST_PARTICLE_EMITTER_COMPONENT_H
#define STARDUST_PARTICLE_EMITTER_COMPONENT_H

#include "stardust/particles/ParticleSystem.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    namespace components
    {
        struct ParticleEmitter final
        {
            ObserverPointer<ParticleSystem> particleSystem = nullptr;
        };
    }
}

#endif
