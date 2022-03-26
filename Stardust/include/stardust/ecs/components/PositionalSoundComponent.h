#pragma once
#ifndef STARDUST_POSITIONAL_SOUND_COMPONENT_H
#define STARDUST_POSITIONAL_SOUND_COMPONENT_H

#include "stardust/audio/source/PositionalSoundSource.h"

namespace stardust
{
    namespace components
    {
        struct PositionalSound final
        {
            audio::PositionalSoundSource source;
        };
    }
}

#endif
