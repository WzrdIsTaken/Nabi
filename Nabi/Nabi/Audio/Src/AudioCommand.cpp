#include "EngineCore.h"

#include "AudioCommand.h"

#include "AudioEffect.h"
#include "AudioSourceVoice.h"
#include "DebugUtils.h"
#include "DirectXUtils.h"
#include "StringUtils.h"
#include "XAudioObjects.h"

#pragma comment(lib, "xaudio2.lib")

namespace nabi::Audio
{
    AudioCommand::AudioCommand(XAudioObjects& xaudioObjects) NABI_NOEXCEPT
        : m_XAudioObjects(xaudioObjects)
    {
        // Initialise XAudio2
        DX_ASSERT(XAudio2Create(
			&m_XAudioObjects.m_XAudio2, // XAudio
            0u,                         // Flags
            XAUDIO2_DEFAULT_PROCESSOR   // Processor
        ));
        DX_ASSERT(m_XAudioObjects.m_XAudio2->CreateMasteringVoice(&m_XAudioObjects.m_MasteringVoice));
        m_XAudioObjects.m_MasteringVoice->GetVoiceDetails(&m_XAudioObjects.m_MasteringVoiceDetails);

        // Initialise X3DAudio
        DWORD dwChannelMask;
        m_XAudioObjects.m_MasteringVoice->GetChannelMask(&dwChannelMask);
        DX_ASSERT(X3DAudioInitialize(
            dwChannelMask,             // Speaker channel mask
            X3DAUDIO_SPEED_OF_SOUND,   // Speed of sound
            m_XAudioObjects.m_X3DAudio // XAudio3D
        ));

    }

    AudioCommand::~AudioCommand() NABI_NOEXCEPT
    {
		m_XAudioObjects.m_MasteringVoice->DestroyVoice(); m_XAudioObjects.m_MasteringVoice = nullptr;
		m_XAudioObjects.m_XAudio2->StopEngine();          m_XAudioObjects.m_XAudio2 = nullptr;
    }

	void AudioCommand::LoadAudioEffect(AudioEffect& audioEffect, std::string const& filepath, LoadSettings const& loadSettings) const NABI_NOEXCEPT
	{
		// Load the riff
		HANDLE file = CreateFile(
			STRING_TO_LPCWSTR_INLINE(filepath), // File name
			GENERIC_READ,                       // Access type
			FILE_SHARE_READ,                    // Share mode
			NULL,                               // Security attributes
			OPEN_EXISTING,                      // Creation disposition
			0ul,                                // Flags
			NULL);                              // Template file

		DX_ASSERT(file);
		DX_ASSERT(SetFilePointer(file, 0l, NULL, FILE_BEGIN));

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		DX_ASSERT(FindChunk(file, c_FourccRIFF, dwChunkSize, dwChunkPosition));

		DWORD fileType;
		DX_ASSERT(ReadChunkData(file, &fileType, sizeof(DWORD), dwChunkPosition));
		ASSERT_FATAL(fileType == c_FourccWAVE, "Trying to load an audio file which is not .wav");

		DX_ASSERT(FindChunk(file, c_FourccFMT, dwChunkSize, dwChunkPosition));
		DX_ASSERT(ReadChunkData(file, &audioEffect.m_WFX, dwChunkSize, dwChunkPosition));

		DX_ASSERT(FindChunk(file, c_FourccDATA, dwChunkSize, dwChunkPosition));
		audioEffect.m_DataBuffer = std::make_unique<BYTE[]>(dwChunkSize);
		DX_ASSERT(ReadChunkData(file, audioEffect.m_DataBuffer.get(), dwChunkSize, dwChunkPosition));

		audioEffect.m_Buffer.AudioBytes = dwChunkSize;
		audioEffect.m_Buffer.pAudioData = audioEffect.m_DataBuffer.get();
		audioEffect.m_Buffer.Flags      = XAUDIO2_END_OF_STREAM;

		if (loadSettings.m_Loop)
		{
			audioEffect.m_Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
	}

	void AudioCommand::LoadAudioVoice(AudioEffect& audioEffect, AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT
	{
		DX_ASSERT(m_XAudioObjects.m_XAudio2->CreateSourceVoice(audioSourceVoice.GetSourceVoiceAddress(), reinterpret_cast<WAVEFORMATEX*>(&audioEffect.m_WFX)));
		DX_ASSERT(audioSourceVoice.SubmitBuffer(audioEffect.m_Buffer));
	}

	void AudioCommand::DestroyAudioEffect(AudioEffect& audioEffect) const NABI_NOEXCEPT
	{
		audioEffect.m_DataBuffer.reset();
	}

	void AudioCommand::DestroyAudioSourceVoice(AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT
	{
		IXAudio2SourceVoice* sourceVoice = audioSourceVoice.GetSourceVoice();
		if (sourceVoice)
		{
			DX_ASSERT(sourceVoice->Stop());
			DX_ASSERT(sourceVoice->FlushSourceBuffers());
			sourceVoice->DestroyVoice();
			sourceVoice = nullptr;
		}	
	}

	void AudioCommand::SetListener(dx::XMFLOAT3 const& transformPosition, dx::XMFLOAT3 const& transformRotation, dx::XMFLOAT3 const& velocity, 
		dx::XMFLOAT3 const& worldForward, dx::XMFLOAT3 const& worldUp) const NABI_NOEXCEPT
	{
		dx::XMFLOAT3 const listenerForward = DirectXUtils::Float3Normalize(DirectXUtils::Float3Rotate(transformRotation, worldForward));
		dx::XMFLOAT3 const listenerUp = DirectXUtils::Float3Normalize(DirectXUtils::Float3Rotate(transformRotation, worldUp));

		X3DAUDIO_LISTENER& listener = m_XAudioObjects.m_Listener;
		listener.OrientFront.x = listenerForward.x;
		listener.OrientFront.y = listenerForward.y;
		listener.OrientFront.z = listenerForward.z;
		listener.OrientTop.x   = listenerUp.x;
		listener.OrientTop.y   = listenerUp.y;
		listener.OrientTop.z   = listenerUp.z;
		listener.Position.x    = transformPosition.x;
		listener.Position.y    = transformPosition.y;
		listener.Position.z    = transformPosition.z;
		listener.Velocity.x    = velocity.x;
		listener.Velocity.y    = velocity.y;
		listener.Velocity.z    = velocity.z;
	}

    HRESULT AudioCommand::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) const NABI_NOEXCEPT
    {
		HRESULT hr = S_OK;
		DX_ASSERT(SetFilePointer(
			hFile,     // File
			0l,        // Distance to move
			NULL,      // Distance to move high
			FILE_BEGIN // Move method
		));
			
		DWORD dwRIFFDataSize = 0ul;
		DWORD bytesRead      = 0ul;
		DWORD dwOffset       = 0ul;
		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwFileType;

		while (hr == S_OK)
		{
			DWORD dwRead;
			DX_ASSERT(ReadFile(
				hFile,		   // File
				&dwChunkType,  // Buffer
				sizeof(DWORD), // Number of bytes to read
				&dwRead,       // Number of bytes read
				NULL           // Overlapped?
			));

			DX_ASSERT(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)); // ""

			switch (dwChunkType)
			{
			case c_FourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4ul;

				DX_ASSERT(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)); //  ""
				break;

			default:
				DX_ASSERT(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)); //  ""
				break;
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) // bytesRead is never set??
			{
				return S_FALSE;
			}
		}

		return S_OK;
    }

	HRESULT AudioCommand::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) const NABI_NOEXCEPT
	{
		DX_ASSERT(SetFilePointer(
			hFile,        // File
			bufferoffset, // Distance to move
			NULL,         // Distance to move high
			FILE_BEGIN    // Move method
		));

		DWORD dwRead;
		DX_ASSERT(ReadFile(
			hFile,      // File
			buffer,     // Buffer
			buffersize, // Number of bytes to read
			&dwRead,    // Number of bytes read
			NULL        // Overlapped?
		));

		return S_OK; // Would have thrown before this point anyways
	}
} // namespace nabi::Audio
