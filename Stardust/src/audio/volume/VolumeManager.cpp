#include "stardust/audio/volume/VolumeManager.h"

namespace stardust
{
	void VolumeManager::AddVolume(const String& volumeName, const f32 value)
	{
		if (!m_volumes.contains(volumeName))
		{
			m_volumes.insert({ volumeName, value });
		}
		else
		{
			m_volumes[volumeName] = value;
		}
	}

	void VolumeManager::SetVolume(const String& volumeName, const f32 value)
	{
		m_volumes[volumeName] = value;
	}

	[[nodiscard]] f32 VolumeManager::GetVolume(const String& volumeName) const
	{
		return m_volumes.at(volumeName);
	}

	void VolumeManager::ResetVolume(const String& volumeName)
	{
		m_volumes[volumeName] = 1.0f;
	}

	[[nodiscard]] bool VolumeManager::DoesVolumeExist(const String& volumeName) const
	{
		return m_volumes.contains(volumeName);
	}

	void VolumeManager::RemoveVolume(const String& volumeName)
	{
		if (volumeName != s_MasterVolumeName)
		{
			m_volumes.erase(volumeName);
		}
	}

	void VolumeManager::ClearAllVolumes()
	{
		const f32 masterVolume = m_volumes[GetMasterVolumeName()];

		m_volumes.clear();
		m_volumes[GetMasterVolumeName()] = masterVolume;
	}
}