#include "stardust/audio/recording/RecordingDevice.h"

#include <algorithm>
#include <cstring>

namespace stardust
{
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

		if (m_frequency != m_previousFrequency || m_channelCount != m_previousChannelCount)
		{
			const u32 bytesPerSample = m_channelCount * static_cast<u32>(SDL_AUDIO_BITSIZE(obtainedAudioSpec.format)) / 8u;
			const u32 bytesPerSecond = m_frequency * bytesPerSample;

			m_bufferByteSize = static_cast<usize>(s_RecordingBufferSeconds) * bytesPerSecond;
			m_maxBufferByteOffset = static_cast<usize>(s_MaxRecordingSeconds) * bytesPerSecond;

			m_recordingBuffer.resize(m_bufferByteSize, 0u);
		}

		return Status::Success;
	}

	void RecordingDevice::Close() noexcept
	{
		if (m_isRecording)
		{
			StopRecording();
		}

		SDL_CloseAudioDevice(m_internalID);
		m_internalID = s_InvalidDeviceID;
	}

	void RecordingDevice::StartRecording()
	{
		std::ranges::fill(m_recordingBuffer, 0u);
		m_currentBufferByteOffset = 0u;

		SDL_PauseAudioDevice(m_internalID, SDL_FALSE);
		m_isRecording = true;
	}

	Pair<Vector<ubyte>, usize> RecordingDevice::StopRecording()
	{
		SDL_PauseAudioDevice(m_internalID, SDL_TRUE);
		m_isRecording = false;

		return { m_recordingBuffer, m_currentBufferByteOffset };
	}

	void RecordingDevice::AudioRecordingCallback(void* const userData, u8* const stream, const i32 length)
	{
		RecordingDevice* const recordingDevice = static_cast<RecordingDevice*>(userData);

		std::memcpy(recordingDevice->m_recordingBuffer.data() + recordingDevice->m_currentBufferByteOffset, stream, length);
		recordingDevice->m_currentBufferByteOffset += static_cast<u32>(length);

		if (recordingDevice->m_currentBufferByteOffset > recordingDevice->m_maxBufferByteOffset)
		{
			recordingDevice->m_callback(recordingDevice->m_recordingBuffer, recordingDevice->m_currentBufferByteOffset);
			recordingDevice->m_currentBufferByteOffset = 0u;
		}
	}
}