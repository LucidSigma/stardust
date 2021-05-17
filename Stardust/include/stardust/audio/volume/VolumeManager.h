#pragma once
#ifndef STARDUST_VOLUME_MANAGER_H
#define STARDUST_VOLUME_MANAGER_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    class VolumeManager
    {
    private:
        static constexpr StringView s_MasterVolumeName = "master";

        HashMap<String, f32> m_volumes{ };

    public:
        [[nodiscard]] inline static String GetMasterVolumeName() noexcept { return String(s_MasterVolumeName); }

        VolumeManager() = default;
        ~VolumeManager() noexcept = default;

        void AddVolume(const String& volumeName, const f32 value = 1.0f);
        void SetVolume(const String& volumeName, const f32 value);
        [[nodiscard]] f32 GetVolume(const String& volumeName) const;
        void ResetVolume(const String& volumeName);

        [[nodiscard]] bool DoesVolumeExist(const String& volumeName) const;

        void RemoveVolume(const String& volumeName);
        void ClearAllVolumes();

        [[nodiscard]] inline f32 operator [](const String& volumeName) const { return GetVolume(volumeName); }

        [[nodiscard]] inline HashMap<String, f32>& GetVolumes() noexcept { return m_volumes; }
        [[nodiscard]] inline const HashMap<String, f32>& GetVolumes() const noexcept { return m_volumes; }
    };
}

#endif