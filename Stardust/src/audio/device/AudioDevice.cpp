#include "stardust/audio/device/AudioDevice.h"

#include <cstring>

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

	AudioDevice::AudioDevice(const i32 index, const bool isRecording)
		: m_index(index), m_name(SDL_GetAudioDeviceName(index, static_cast<SDL_bool>(isRecording))), m_isRecordingDevice(isRecording)
	{ }

	AudioDevice::~AudioDevice() noexcept
	{
		if (!IsOpen())
		{
			Close();
		}
	}

	void AudioDevice::Open()
	{
		const SDL_AudioSpec desiredAudioSpec{
			.freq = 44'100,
			.format = AUDIO_F32,
			.channels = 2u,
			.silence = 0u,
			.samples = 4'096u,
			.size = 0u,
			.callback = m_isRecordingDevice ? AudioRecordingCallback : AudioPlaybackCallback,
			.userdata = nullptr,
		};

		m_internalID = SDL_OpenAudioDevice(
			m_name.c_str(), static_cast<SDL_bool>(m_isRecordingDevice),
			&desiredAudioSpec, nullptr,
			SDL_AUDIO_ALLOW_FORMAT_CHANGE
		);
	}

	void AudioDevice::Close() noexcept
	{
		SDL_CloseAudioDevice(m_internalID);
		m_internalID = s_InvalidDeviceID;
	}

	void AudioDevice::AudioPlaybackCallback(void* const userData, u8* const stream, const i32 length)
	{ }

	void AudioDevice::AudioRecordingCallback(void* const userData, u8* const stream, const i32 length)
	{ }
}