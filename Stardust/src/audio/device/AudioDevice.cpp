#include "stardust/audio/device/AudioDevice.h"

#include <SDL2/SDL.h>

namespace stardust
{
	[[nodiscard]] Vector<AudioDevice> AudioDevice::GetPlaybackDevices()
	{
		const i32 audioPlaybackDeviceCount = SDL_GetNumAudioDevices(SDL_FALSE);
		Vector<AudioDevice> audioPlaybackDevices;
		audioPlaybackDevices.reserve(static_cast<usize>(audioPlaybackDeviceCount));

		for (i32 i = 0; i < audioPlaybackDeviceCount; ++i)
		{
			audioPlaybackDevices.emplace_back(i, false);
		}

		return audioPlaybackDevices;
	}
	
	[[nodiscard]] Vector<AudioDevice> AudioDevice::GetRecordingDevices()
	{
		const i32 audioRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);
		Vector<AudioDevice> audioRecordingDevices;
		audioRecordingDevices.reserve(static_cast<usize>(audioRecordingDeviceCount));

		for (i32 i = 0; i < audioRecordingDeviceCount; ++i)
		{
			audioRecordingDevices.emplace_back(i, true);
		}

		return audioRecordingDevices;
	}

	AudioDevice::AudioDevice(const i32 id, const bool isRecording)
		: m_id(id), m_name(SDL_GetAudioDeviceName(id, static_cast<SDL_bool>(isRecording))), m_isRecordingDevice(isRecording)
	{ }
}