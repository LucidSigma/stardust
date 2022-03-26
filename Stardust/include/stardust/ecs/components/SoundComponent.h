#pragma once
#ifndef STARDUST_SOUND_COMPONENT_H
#define STARDUST_SOUND_COMPONENT_H

#include "stardust/audio/source/SoundSource.h"

namespace stardust
{
    namespace components
    {
        struct Sound final
        {
            audio::SoundSource source;
        };
    }
}

#endif
