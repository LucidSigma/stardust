#pragma once
#ifndef STARDUST_SOUND_CUE_PLAYER_H
#define STARDUST_SOUND_CUE_PLAYER_H

#include "stardust/audio/mixing/MixingBus.h"
#include "stardust/audio/sounds/Sounds.h"
#include "stardust/audio/source/PositionalSoundSource.h"
#include "stardust/audio/source/SoundSource.h"
#include "stardust/audio/volume/VolumeManager.h"
#include "stardust/audio/Audio.h"
#include "stardust/audio/SoundSystem.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        class SoundCuePlayer final
        {
        private:
            ObserverPointer<SoundSystem> m_soundSystem = nullptr;
            ObserverPointer<const VolumeManager> m_volumeManager = nullptr;

            HashMap<String, List<Variant<ObserverPointer<Sound>, ObserverPointer<SoundStream>>>> m_soundCues{ };

        public:
            auto Initialise(SoundSystem& soundSystem, const VolumeManager& volumeManager) -> void;

            auto PlayCue(const String& cueName, const SoundPlayData soundPlayData = SoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) const -> SoundSource;
            auto PlayCueClocked(const String& cueName, const f64 fixedTimestep, const SoundPlayData soundPlayData = SoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) const -> SoundSource;
            auto PlayCuePositional(const String& cueName, const PositionalSoundPlayData soundPlayData = PositionalSoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) const -> PositionalSoundSource;
            auto PlayCuePositionalClocked(const String& cueName, const f64 fixedTimestep, const PositionalSoundPlayData soundPlayData = PositionalSoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) const -> PositionalSoundSource;

            auto PlayCueWithVolume(const String& cueName, const String& volumeName, SoundPlayData soundPlayData = SoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) -> SoundSource;
            auto PlayCueClockedWithVolume(const String& cueName, const String& volumeName, const f64 fixedTimestep, SoundPlayData soundPlayData = SoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) -> SoundSource;
            auto PlayCuePositionalWithVolume(const String& cueName, const String& volumeName, PositionalSoundPlayData soundPlayData = PositionalSoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) const -> PositionalSoundSource;
            auto PlayCuePositionalClockedWithVolume(const String& cueName, const String& volumeName, const f64 fixedTimestep, PositionalSoundPlayData soundPlayData = PositionalSoundPlayData{ }, const ObserverPointer<MixingBus> mixingBus = nullptr) const -> PositionalSoundSource;

            auto AddSoundToCue(const String& cueName, Sound& sound) -> void;
            auto AddSoundStreamToCue(const String& cueName, SoundStream& soundStream) -> void;

            [[nodiscard]] inline auto DoesCueExist(const String& cueName) const -> bool { return m_soundCues.contains(cueName); }
            inline auto RemoveCue(const String& cueName) -> void { m_soundCues.erase(cueName); }

            [[nodiscard]] inline auto GetAllCues() const noexcept -> const decltype(m_soundCues)& { return m_soundCues; }
            [[nodiscard]] inline auto GetSoundsInCue(const String& cueName) const -> const List<Variant<ObserverPointer<Sound>, ObserverPointer<SoundStream>>>& { return m_soundCues.at(cueName); }

        private:
            [[nodiscard]] auto GetRandomSoundFromCue(const String& cueName) const -> const Variant<ObserverPointer<Sound>, ObserverPointer<SoundStream>>&;
        };
    }
}

#endif
