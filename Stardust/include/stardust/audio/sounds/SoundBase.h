#pragma once
#ifndef STARDUST_SOUND_BASE_H
#define STARDUST_SOUND_BASE_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <concepts>

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#undef min
#undef max

#include "stardust/audio/sounds/SoundEnums.h"
#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/filesystem/vfs/VFS.h"

namespace stardust
{
    namespace audio
    {
        template <std::derived_from<SoLoud::AudioSource> T>
        class SoundBase
            : private INoncopyable, private INonmovable
        {
        private:
            T m_handle;
            bool m_isValid = false;

            f64 m_length = 0.0f;
            bool m_isSingleInstance = false;

            f32 m_volume = 1.0f;
            InaudibleBehaviour m_inaudibleBehaviour = InaudibleBehaviour::KeepTicking;

            bool m_isLooping = false;
            f32 m_loopPoint = 0.0f;

            f32 m_dopplerFactor = 1.0f;
            AttenuationModel m_attenuationModel = AttenuationModel::None;
            f32 m_attenuationRolloffFactor = 1.0f;

            bool m_isDistanceDelayEnabled = false;
            bool m_isRelativeToListener = false;

            f32 m_minDistance = 1.0f;
            f32 m_maxDistance = 1'000'000.0f;

        public:
            SoundBase() = default;

            explicit SoundBase(const StringView& filepath)
            {
                Initialise(filepath);
            }

            template <std::same_as<SoLoud::Wav> U = T>
            explicit SoundBase(const Vector<ubyte>& pcmData, const f32 frequency = 44'100u, const u32 channelCount = 1u)
            {
                Initialise(pcmData, frequency, channelCount);
            }

            ~SoundBase() noexcept = default;

            void Initialise(const StringView& filepath)
            {
                const Vector<ubyte> rawSoundData = vfs::ReadFileData(filepath);

                const SoLoud::result loadStatus = m_handle.loadMem(
                    reinterpret_cast<const unsigned char*>(rawSoundData.data()),
                    static_cast<u32>(rawSoundData.size()),
                    true, false
                );
                m_isValid = loadStatus == 0u;

                if (m_isValid)
                {
                    m_length = m_handle.getLength();
                    SetInaudibleBehaviour(InaudibleBehaviour::KeepTicking);
                }
            }

            template <std::same_as<SoLoud::Wav> U = T>
            void Initialise(Vector<ubyte> pcmData, const f32 frequency = 44'100u, const u32 channelCount = 1u)
            {
                const SoLoud::result loadStatus = m_handle.loadRawWave(
                    reinterpret_cast<f32*>(pcmData.data()),
                    static_cast<u32>(pcmData.size() / sizeof(f32)),
                    static_cast<f32>(frequency), channelCount,
                    true, false
                );
                m_isValid = loadStatus == 0u;

                if (m_isValid)
                {
                    m_length = m_handle.getLength();
                    SetInaudibleBehaviour(InaudibleBehaviour::KeepTicking);
                }
            }

            [[nodiscard]] inline bool IsValid() const noexcept { return m_isValid; }

            void StopAll()
            {
                m_handle.stop();
            }

            [[nodiscard]] inline f64 GetLength() const noexcept { return m_length; }
            [[nodiscard]] inline bool IsSingleInstance() const noexcept { return m_isSingleInstance; }

            void SetSingleInstance(const bool isSingleInstance)
            {
                m_handle.setSingleInstance(isSingleInstance);
                m_isSingleInstance = isSingleInstance;
            }

            [[nodiscard]] inline f32 GetVolume() const noexcept { return m_volume; }

            void SetVolume(const f32 volume)
            {
                m_handle.setVolume(volume);
                m_volume = volume;
            }

            [[nodiscard]] inline InaudibleBehaviour GetInaudibleBehaviour() const noexcept { return m_inaudibleBehaviour; }

            void SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour)
            {
                switch (inaudibleBehaviour)
                {
                case InaudibleBehaviour::KeepTicking:
                    m_handle.setInaudibleBehavior(true, false);

                    break;

                case InaudibleBehaviour::Kill:
                    m_handle.setInaudibleBehavior(false, true);

                    break;

                case InaudibleBehaviour::Pause:
                default:
                    m_handle.setInaudibleBehavior(false, false);

                    break;
                }

                m_inaudibleBehaviour = inaudibleBehaviour;
            }

            [[nodiscard]] inline bool IsLooping() const noexcept { return m_isLooping; }

            void SetLooping(const bool enableLooping)
            {
                m_handle.setLooping(enableLooping);
                m_isLooping = enableLooping;
            }

            [[nodiscard]] inline f32 GetLoopPoint() const noexcept { return m_loopPoint; }

            void SetLoopPoint(const f32 loopPoint)
            {
                m_handle.setLoopPoint(loopPoint);
                m_loopPoint = loopPoint;
            }

            [[nodiscard]] inline f32 GetDopplerFactor() const noexcept { return m_dopplerFactor; }

            void SetDopplerFactor(const f32 dopplerFactor)
            {
                m_handle.set3dDopplerFactor(dopplerFactor);
                m_dopplerFactor = dopplerFactor;
            }

            [[nodiscard]] inline AttenuationModel GetAttenuationModel() const noexcept { return m_attenuationModel; }

            void SetAttenuationModel(const AttenuationModel attenuationModel)
            {
                m_handle.set3dAttenuation(static_cast<unsigned int>(attenuationModel), m_attenuationRolloffFactor);
                m_attenuationModel = attenuationModel;
            }

            [[nodiscard]] inline f32 GetAttenuationRolloffFactor() const noexcept { return m_attenuationRolloffFactor; }

            void SetAttenuationRolloffFactor(const f32 attenuationRolloffFactor)
            {
                m_handle.set3dAttenuation(static_cast<unsigned int>(m_attenuationModel), attenuationRolloffFactor);
                m_attenuationRolloffFactor = attenuationRolloffFactor;
            }

            [[nodiscard]] inline bool IsDistanceDelayEnabledBy() const noexcept { return m_isDistanceDelayEnabled; }

            void SetDistanceDelayBy(const bool enableDistanceDelay)
            {
                m_handle.set3dDistanceDelay(enableDistanceDelay);
                m_isDistanceDelayEnabled = enableDistanceDelay;
            }

            [[nodiscard]] inline bool IsRelativeToListenerBy() const noexcept { return m_isRelativeToListener; }

            void SetRelativeToListenerBy(const bool isRelativeToListener)
            {
                m_handle.set3dListenerRelative(isRelativeToListener);
                m_isRelativeToListener = isRelativeToListener;
            }

            [[nodiscard]] inline f32 GetMinDistance() const noexcept { return m_minDistance; }

            void SetMinDistance(const f32 minDistance)
            {
                m_handle.set3dMinMaxDistance(minDistance, m_maxDistance);
                m_minDistance = minDistance;
            }

            [[nodiscard]] inline f32 GetMaxDistance() const noexcept { return m_maxDistance; }

            void SetMaxDistance(const f32 maxDistance)
            {
                m_handle.set3dMinMaxDistance(m_minDistance, maxDistance);
                m_maxDistance = maxDistance;
            }

            [[nodiscard]] inline T& GetRawHandle() noexcept { return m_handle; }
            [[nodiscard]] inline const T& GetRawHandle() const noexcept { return m_handle; }
        };
    }
}

#endif