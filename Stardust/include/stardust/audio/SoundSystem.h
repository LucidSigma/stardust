#pragma once
#ifndef STARDUST_SOUND_SYSTEM_H
#define STARDUST_SOUND_SYSTEM_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <soloud/soloud.h>
#undef min
#undef max

#include "stardust/audio/listener/Listener.h"
#include "stardust/audio/sounds/Sounds.h"
#include "stardust/audio/source/PositionalSoundSource.h"
#include "stardust/audio/source/SoundSource.h"
#include "stardust/audio/Audio.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        class SoundSystem final
            : private INoncopyable
        {
        private:
            UniquePointer<SoLoud::Soloud> m_soLoudHandle = nullptr;
            Listener m_listener;

            bool m_didInitialiseSuccessfully = false;
            String m_errorString{ };

        public:
            friend class MixingBus;

            SoundSystem() = default;
            SoundSystem(SoundSystem&& other) noexcept;
            auto operator =(SoundSystem&& other) noexcept -> SoundSystem&;
            ~SoundSystem() noexcept;

            auto Initialise() -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_soLoudHandle != nullptr && m_didInitialiseSuccessfully; }

            auto Update() const -> void;

            auto PlaySound(Sound& sound, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;
            auto PlaySound(SoundStream& soundStream, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;
            auto PlaySoundClocked(Sound& sound, const f64 fixedTimestep, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;
            auto PlaySoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;

            auto PlayPositionalSound(Sound& sound, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;
            auto PlayPositionalSound(SoundStream& soundStream, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;
            auto PlayPositionalSoundClocked(Sound& sound, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;
            auto PlayPositionalSoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;

            auto PauseAllSounds() const -> void;
            auto ResumeAllSounds() const -> void;
            auto StopAllSounds() const noexcept -> void;

            auto FadeGlobalVolume(const f32 targetVolume, const f32 seconds) const -> void;

            auto OscillateGlobalVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency) const -> void;
            auto ResetGlobalVolumeOscillation() const -> void;

            [[nodiscard]] auto GetPlayingSoundCount() const -> u32;
            [[nodiscard]] auto IsAnySoundPlaying() const -> bool;

            [[nodiscard]] auto GetGlobalVolume() const noexcept -> f32;
            auto SetGlobalVolume(const f32 globalVolume) const noexcept -> void;

            [[nodiscard]] auto GetPostClipScaler() const noexcept -> f32;
            auto SetPostClipScaler(const f32 postClipScaler) const noexcept -> void;

            [[nodiscard]] auto GetSpeedOfSound() const noexcept -> f32;
            void SetSpeedOfSound(const f32 speedOfSound) const noexcept;

            [[nodiscard]] inline auto GetListener() noexcept -> Listener& { return m_listener; }
            [[nodiscard]] inline auto GetListener() const noexcept -> const Listener& { return m_listener; }

            [[nodiscard]] auto GetRawHandle() noexcept -> SoLoud::Soloud& { return *m_soLoudHandle.get(); }
            [[nodiscard]] auto GetRawHandle() const noexcept -> const SoLoud::Soloud& { return *m_soLoudHandle.get(); }

            [[nodiscard]] auto GetErrorString() const noexcept -> const String& { return m_errorString; }

        private:
            auto SetVoiceHandleParameters(const VoiceHandle voiceHandle, const SoundPlayData& soundPlayData) const -> void;
            auto SetVoiceHandleParameters(const VoiceHandle voiceHandle, const PositionalSoundPlayData& soundPlayData) const -> void;
        };
    }
}

#endif
