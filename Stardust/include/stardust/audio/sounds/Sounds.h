#pragma once
#ifndef STARDUST_SOUNDS_H
#define STARDUST_SOUNDS_H

#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>
#undef min
#undef max

#include "SoundBase.h"

namespace stardust
{
    namespace audio
    {
        using Sound = SoundBase<SoLoud::Wav>;
        using SoundStream = SoundBase<SoLoud::WavStream>;
    }
}

#endif