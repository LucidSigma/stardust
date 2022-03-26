#include "stardust/audio/SoundCuePlayer.h"

#include <variant>

#include "stardust/math/random/Random.h"
#include "stardust/utility/Utility.h"

namespace stardust
{
    namespace audio
    {
        auto SoundCuePlayer::Initialise(SoundSystem& soundSystem, const VolumeManager& volumeManager) -> void
        {
            m_soundSystem = &soundSystem;
            m_volumeManager = &volumeManager;
        }

        auto SoundCuePlayer::PlayCue(const String& cueName, const SoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) const -> SoundSource
        {
            const auto& soundVariant = GetRandomSoundFromCue(cueName);

            return std::visit(
                utility::VariantOverloader{
                    [this, soundPlayData, mixingBus](const ObserverPointer<Sound> sound) -> SoundSource
                    {
                        if (mixingBus != nullptr)
                        {
                            return mixingBus->PlaySound(*sound, soundPlayData);
                        }

                        return m_soundSystem->PlaySound(*sound, soundPlayData);
                    },
                    [this, soundPlayData, mixingBus](const ObserverPointer<SoundStream> soundStream) -> SoundSource
                    {
                        if (mixingBus != nullptr)
                        {
                            return mixingBus->PlaySound(*soundStream, soundPlayData);
                        }

                        return m_soundSystem->PlaySound(*soundStream, soundPlayData);
                    },
                },
                soundVariant
            );
        }

        auto SoundCuePlayer::PlayCueClocked(const String& cueName, const f64 fixedTimestep, const SoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) const -> SoundSource
        {
            const auto& soundVariant = GetRandomSoundFromCue(cueName);

            return std::visit(
                utility::VariantOverloader{
                    [this, fixedTimestep, soundPlayData, mixingBus](const ObserverPointer<Sound> sound) -> SoundSource
                    {
                        if (mixingBus != nullptr)
                        {
                            return mixingBus->PlaySoundClocked(*sound, fixedTimestep, soundPlayData);
                        }

                        return m_soundSystem->PlaySoundClocked(*sound, fixedTimestep, soundPlayData);
                    },
                    [this, fixedTimestep, soundPlayData, mixingBus](const ObserverPointer<SoundStream> soundStream) -> SoundSource
                    {
                        if (mixingBus != nullptr)
                        {
                            return mixingBus->PlaySoundClocked(*soundStream, fixedTimestep, soundPlayData);
                        }

                        return m_soundSystem->PlaySoundClocked(*soundStream, fixedTimestep, soundPlayData);
                    },
                },
                soundVariant
            );
        }

        auto SoundCuePlayer::PlayCuePositional(const String& cueName, const PositionalSoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) const -> PositionalSoundSource
        {
            const auto& soundVariant = GetRandomSoundFromCue(cueName);

            return std::visit(
                utility::VariantOverloader{
                    [this, soundPlayData, mixingBus](const ObserverPointer<Sound> sound) -> PositionalSoundSource
                    {
                    if (mixingBus != nullptr)
                        {
                            return mixingBus->PlayPositionalSound(*sound, soundPlayData);
                        }

                        return m_soundSystem->PlayPositionalSound(*sound, soundPlayData);
                    },
                    [this, soundPlayData, mixingBus](const ObserverPointer<SoundStream> soundStream) -> PositionalSoundSource
                    {
                        if (mixingBus != nullptr)
                        {
                            return mixingBus->PlayPositionalSound(*soundStream, soundPlayData);
                        }

                        return m_soundSystem->PlayPositionalSound(*soundStream, soundPlayData);
                    },
                },
                soundVariant
            );
        }

        auto SoundCuePlayer::PlayCuePositionalClocked(const String& cueName, const f64 fixedTimestep, const PositionalSoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) const -> PositionalSoundSource
        {
            const auto& soundVariant = GetRandomSoundFromCue(cueName);

            return std::visit(
                utility::VariantOverloader{
                    [this, fixedTimestep, soundPlayData, mixingBus](const ObserverPointer<Sound> sound) -> PositionalSoundSource
                    {
                        if (mixingBus != nullptr)
                        {
                            return mixingBus->PlayPositionalSoundClocked(*sound, fixedTimestep, soundPlayData);
                        }

                        return m_soundSystem->PlayPositionalSoundClocked(*sound, fixedTimestep, soundPlayData);
                    },
                    [this, fixedTimestep, soundPlayData, mixingBus](const ObserverPointer<SoundStream> soundStream) -> PositionalSoundSource
                    {
                        if (mixingBus != nullptr)
                        {
                            return mixingBus->PlayPositionalSoundClocked(*soundStream, fixedTimestep, soundPlayData);
                        }

                        return m_soundSystem->PlayPositionalSoundClocked(*soundStream, fixedTimestep, soundPlayData);
                    },
                },
                soundVariant
            );
        }

        auto SoundCuePlayer::PlayCueWithVolume(const String& cueName, const String& volumeName, SoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) -> SoundSource
        {
            soundPlayData.volume *= m_volumeManager->GetVolume(volumeName) * m_volumeManager->GetVolume(m_volumeManager->GetMasterVolumeName());

            return PlayCue(cueName, soundPlayData, mixingBus);
        }

        auto SoundCuePlayer::PlayCueClockedWithVolume(const String& cueName, const String& volumeName, const f64 fixedTimestep, SoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) -> SoundSource
        {
            soundPlayData.volume *= m_volumeManager->GetVolume(volumeName) * m_volumeManager->GetVolume(m_volumeManager->GetMasterVolumeName());

            return PlayCueClocked(cueName, fixedTimestep, soundPlayData, mixingBus);
        }

        auto SoundCuePlayer::PlayCuePositionalWithVolume(const String& cueName, const String& volumeName, PositionalSoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) const -> PositionalSoundSource
        {
            soundPlayData.volume *= m_volumeManager->GetVolume(volumeName) * m_volumeManager->GetVolume(m_volumeManager->GetMasterVolumeName());

            return PlayCuePositional(cueName, soundPlayData, mixingBus);
        }

        auto SoundCuePlayer::PlayCuePositionalClockedWithVolume(const String& cueName, const String& volumeName, const f64 fixedTimestep, PositionalSoundPlayData soundPlayData, const ObserverPointer<MixingBus> mixingBus) const -> PositionalSoundSource
        {
            soundPlayData.volume *= m_volumeManager->GetVolume(volumeName) * m_volumeManager->GetVolume(m_volumeManager->GetMasterVolumeName());

            return PlayCuePositionalClocked(cueName, fixedTimestep, soundPlayData, mixingBus);
        }
        
        auto SoundCuePlayer::AddSoundToCue(const String& cueName, Sound& sound) -> void
        {
            if (!m_soundCues.contains(cueName))
            {
                m_soundCues[cueName] = { };
            }

            m_soundCues[cueName].push_back(&sound);
        }

        auto SoundCuePlayer::AddSoundStreamToCue(const String& cueName, SoundStream& soundStream) -> void
        {
            if (!m_soundCues.contains(cueName))
            {
                m_soundCues[cueName] = { };
            }

            m_soundCues[cueName].push_back(&soundStream);
        }

        auto SoundCuePlayer::GetRandomSoundFromCue(const String& cueName) const -> const Variant<ObserverPointer<Sound>, ObserverPointer<SoundStream>>&
        {
            const auto& sounds = m_soundCues.at(cueName);
            const usize randomIndex = Random::GenerateInt<usize>(0u, sounds.size());

            return sounds.at(randomIndex);
        }
    }
}
