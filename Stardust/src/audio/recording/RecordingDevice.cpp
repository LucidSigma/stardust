#include "stardust/audio/recording/RecordingDevice.h"

namespace stardust
{
	namespace
	{
		void AudioRecordingCallback(void* const userData, u8* const stream, const i32 length)
		{

		}
	}

	[[nodiscard]] Vector<RecordingDevice::Info> RecordingDevice::GetAllDeviceInfos()
	{
		const i32 audioRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);
		Vector<Info> audioRecordingDevices{ };
		audioRecordingDevices.reserve(static_cast<usize>(audioRecordingDeviceCount));

		for (i32 i = 0; i < audioRecordingDeviceCount; ++i)
		{
			audioRecordingDevices.push_back(Info{
				.index = i,
				.name = SDL_GetAudioDeviceName(i, SDL_TRUE),
			});
		}

		return audioRecordingDevices;
	}

	RecordingDevice::RecordingDevice(const Info& info)
		: m_index(info.index), m_name(info.name)
	{ }

	RecordingDevice::~RecordingDevice() noexcept
	{
		if (IsOpen())
		{
			Close();
		}
	}

	Status RecordingDevice::Open(const u32 frequency, const u32 channelCount)
	{
		const SDL_AudioSpec desiredAudioSpec{
			.freq = static_cast<i32>(frequency),
			.format = AUDIO_F32,
			.channels = static_cast<u8>(channelCount),
			.silence = 0u,
			.samples = 4'096u,
			.size = 0u,
			.callback = AudioRecordingCallback,
			.userdata = this,
		};

		SDL_AudioSpec obtainedAudioSpec{ };

		m_internalID = SDL_OpenAudioDevice(
			m_name.c_str(), SDL_TRUE,
			&desiredAudioSpec, &obtainedAudioSpec,
			SDL_AUDIO_ALLOW_FORMAT_CHANGE
		);

		if (!IsOpen())
		{
			return Status::Fail;
		}

		m_frequency = static_cast<u32>(obtainedAudioSpec.freq);
		m_channelCount = static_cast<u32>(obtainedAudioSpec.channels);

		return Status::Success;
	}

	void RecordingDevice::Close() noexcept
	{
		SDL_CloseAudioDevice(m_internalID);
		m_internalID = s_InvalidDeviceID;
	}
}