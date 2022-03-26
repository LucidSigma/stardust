#pragma once
#ifndef STARDUST_SOUND_BASE_H
#define STARDUST_SOUND_BASE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <concepts>
#include <type_traits>
#include <utility>

#include <soloud/soloud.h>
#include <soloud/soloud_audiosource.h>
#undef min
#undef max

#include "stardust/audio/Audio.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        template <std::derived_from<SoLoud::AudioSource> Source>
        class SoundBase
            : private INoncopyable
        {
        private:
            Source m_handle;
            bool m_isValid = false;

            f64 m_length = 0.0f;
            bool m_isSingleInstance = false;

        public:
            SoundBase() = default;

            explicit SoundBase(const StringView filepath)
            {
                Initialise(filepath);
            }

            SoundBase(SoundBase&&) noexcept = default;
            auto operator =(SoundBase&&) noexcept -> SoundBase& = default;

            virtual ~SoundBase() noexcept = default;

            auto Initialise(const StringView filepath) -> void
            {
                auto rawSoundDataResult = vfs::ReadFileBytes(filepath);

                if (rawSoundDataResult.is_err())
                {
                    return;
                }

                const List<ubyte> rawSoundData = std::move(rawSoundDataResult).unwrap();

                const SoLoud::result loadStatus = m_handle.loadMem(
                    reinterpret_cast<const ubyte*>(rawSoundData.data()),
                    static_cast<u32>(rawSoundData.size()),
                    true,
                    false
                );
                m_isValid = loadStatus == 0u;

                if (m_isValid)
                {
                    m_length = m_handle.getLength();
                }
            }

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_isValid; }

            auto StopAll() noexcept -> void
            {
                m_handle.stop();
            }

            [[nodiscard]] inline auto GetLength() const noexcept -> f64 { return m_length; }

            [[nodiscard]] inline auto IsSingleInstance() const noexcept -> bool { return m_isSingleInstance; }

            auto SetSingleInstance(const bool isSingleInstance) -> void
            {
                m_handle.setSingleInstance(isSingleInstance);
                m_isSingleInstance = isSingleInstance;
            }

            [[nodiscard]] inline auto GetRawHandle() noexcept -> Source& { return m_handle; }
            [[nodiscard]] inline auto GetRawHandle() const noexcept -> const Source& { return m_handle; }
        };
    }
}

#endif
