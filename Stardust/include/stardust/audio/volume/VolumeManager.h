#pragma once
#ifndef STARDUST_VOLUME_MANAGER_H
#define STARDUST_VOLUME_MANAGER_H

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        class VolumeManager final
        {
        private:
            static constexpr const char* s_MasterVolumeName = "master";

            HashMap<String, f32> m_volumes{ };

        public:
            [[nodiscard]] inline static auto GetMasterVolumeName() noexcept -> String { return String(s_MasterVolumeName); }

            VolumeManager();

            [[nodiscard]] inline auto GetMasterVolume() const -> f32 { return m_volumes.at(s_MasterVolumeName); }

            auto AddVolume(const String& volumeName, const f32 value = 1.0f) -> void;
            auto SetVolume(const String& volumeName, const f32 value) -> void;
            [[nodiscard]] auto GetVolume(const String& volumeName) const -> f32;
            auto ResetVolume(const String& volumeName) -> void;

            [[nodiscard]] auto DoesVolumeExist(const String& volumeName) const -> bool;

            auto RemoveVolume(const String& volumeName) -> void;
            auto ClearAllVolumes() -> void;

            [[nodiscard]] inline auto operator [](const String& volumeName) const -> f32 { return GetVolume(volumeName); }

            [[nodiscard]] inline auto GetVolumes() noexcept -> HashMap<String, f32>& { return m_volumes; }
            [[nodiscard]] inline auto GetVolumes() const noexcept -> const HashMap<String, f32>& { return m_volumes; }
        };
    }
}

#endif
