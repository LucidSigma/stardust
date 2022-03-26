#pragma once
#ifndef STARDUST_AUDIO_H
#define STARDUST_AUDIO_H

#include <limits>
#include <type_traits>

#include <soloud/soloud.h>
#undef min
#undef max

#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        using VoiceHandle = SoLoud::handle;

        constexpr VoiceHandle InvalidVoiceHandle = std::numeric_limits<VoiceHandle>::max();

        enum class AttenuationModel
            : std::underlying_type_t<SoLoud::AudioSource::ATTENUATION_MODELS>
        {
            None = SoLoud::AudioSource::NO_ATTENUATION,
            Inverse = SoLoud::AudioSource::INVERSE_DISTANCE,
            Linear = SoLoud::AudioSource::LINEAR_DISTANCE,
            Exponential = SoLoud::AudioSource::EXPONENTIAL_DISTANCE,
        };

        enum class InaudibleBehaviour
        {
            KeepTicking,
            Pause,
            Kill,
        };

        struct AttenuationInfo final
        {
            AttenuationModel model = AttenuationModel::None;
            f32 rolloffFactor = 0.0f;

            f32 minDistance = 1.0f;
            f32 maxDistance = 1'000.0f;
        };

        struct SoundPlayData final
        {
            f32 volume = 1.0f;
            f32 pan = 0.0f;
            f32 relativePlaySpeed = 1.0f;

            bool loop = false;
            bool startPaused = false;

            bool protect = false;
        };

        struct PositionalSoundPlayData final
        {
            f32 volume = 1.0f;
            f32 relativePlaySpeed = 1.0f;

            Vector2 position = Vector2Zero;
            Vector2 velocity = Vector2Zero;

            AttenuationInfo attenuation{ };
            InaudibleBehaviour inaudibleBehaviour = InaudibleBehaviour::Kill;

            bool loop = false;
            bool startPaused = false;

            bool protect = false;
        };

        constexpr f32 DefaultVolume = -1.0f;
    }
}

#endif
