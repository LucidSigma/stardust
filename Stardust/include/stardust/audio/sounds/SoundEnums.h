#pragma once
#ifndef STARDUST_SOUND_ENUMS_H
#define STARDUST_SOUND_ENUMS_H

#include <type_traits>

#include <soloud/soloud_audiosource.h>
#undef min
#undef max

namespace stardust
{
    enum class InaudibleBehaviour
    {
        Pause,
        KeepTicking,
        Kill,
    };

    enum class AttenuationModel
        : std::underlying_type_t<SoLoud::AudioSource::ATTENUATION_MODELS>
    {
        None = SoLoud::AudioSource::NO_ATTENUATION,
        InverseDistance = SoLoud::AudioSource::INVERSE_DISTANCE,
        LinearDistance = SoLoud::AudioSource::LINEAR_DISTANCE,
        ExponentialDistance = SoLoud::AudioSource::EXPONENTIAL_DISTANCE,
    };
}

#endif