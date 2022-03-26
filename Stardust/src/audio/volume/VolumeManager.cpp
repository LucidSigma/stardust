#include "stardust/audio/volume/VolumeManager.h"

#include "stardust/math/Math.h"

namespace stardust
{
    namespace audio
    {
        VolumeManager::VolumeManager()
        {
            AddVolume(s_MasterVolumeName, 1.0f);
        }

        auto VolumeManager::AddVolume(const String& volumeName, const f32 value) -> void
        {
            if (!m_volumes.contains(volumeName))
            {
                m_volumes.insert({ volumeName, glm::clamp(value, 0.0f, 1.0f) });
            }
            else
            {
                m_volumes[volumeName] = glm::clamp(value, 0.0f, 1.0f);
            }
        }

        auto VolumeManager::SetVolume(const String& volumeName, const f32 value) -> void
        {
            m_volumes[volumeName] = glm::clamp(value, 0.0f, 1.0f);
        }

        [[nodiscard]] auto VolumeManager::GetVolume(const String& volumeName) const -> f32
        {
            return m_volumes.at(volumeName);
        }

        auto VolumeManager::ResetVolume(const String& volumeName) -> void
        {
            m_volumes[volumeName] = 1.0f;
        }

        [[nodiscard]] auto VolumeManager::DoesVolumeExist(const String& volumeName) const -> bool
        {
            return m_volumes.contains(volumeName);
        }

        auto VolumeManager::RemoveVolume(const String& volumeName) -> void
        {
            if (volumeName != s_MasterVolumeName)
            {
                m_volumes.erase(volumeName);
            }
        }

        auto VolumeManager::ClearAllVolumes() -> void
        {
            const f32 masterVolume = m_volumes[GetMasterVolumeName()];

            m_volumes.clear();
            m_volumes[s_MasterVolumeName] = masterVolume;
        }
    }
}
