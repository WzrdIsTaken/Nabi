#pragma once
#include "EngineCore.h"
#include "WinCore.h"
#include "DirectXCore.h"
#include "AudioCore.h"

// Super useful resource for this - https://stackoverflow.com/questions/67418892/playing-audio-with-xaudio2-c

namespace nabi::Audio
{
	class AudioSourceVoice;
	struct AudioEffect;
	struct XAudioObjects;
} // namespace nabi::Audio

namespace nabi::Audio
{
	class AudioCommand final
	{
	public:
		struct LoadSettings final
		{
			bool m_Loop;
		};
		static LoadSettings constexpr c_DefaultLoadSettings = 
		{
			.m_Loop = false
		};

		AudioCommand(XAudioObjects& xaudioObjects) NABI_NOEXCEPT;
		~AudioCommand() NABI_NOEXCEPT;

		void LoadAudioEffect(AudioEffect& audioEffect, std::string const& filepath, LoadSettings const& loadSettings) const NABI_NOEXCEPT;
		void LoadAudioVoice(AudioEffect& audioEffect, AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT;

		void DestroyAudioEffect(AudioEffect& audioEffect) const NABI_NOEXCEPT;
		void DestroyAudioSourceVoice(AudioSourceVoice& audioSourceVoice) const NABI_NOEXCEPT;

		void SetListener(dx::XMFLOAT3 const& transformPosition, dx::XMFLOAT3 const& transformRotation, dx::XMFLOAT3 const& velocity, 
			dx::XMFLOAT3 const& worldForward, dx::XMFLOAT3 const& worldUp) const NABI_NOEXCEPT;

	private:
		static DWORD constexpr c_FourccRIFF = 'FFIR';
		static DWORD constexpr c_FourccDATA = 'atad';
		static DWORD constexpr c_FourccFMT  = ' tmf';
		static DWORD constexpr c_FourccWAVE = 'EVAW';
		static DWORD constexpr c_FourccXWMA = 'AMWX';
		static DWORD constexpr c_FourccDPDS = 'sdpd';

		[[nodiscard]] HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) const NABI_NOEXCEPT;
		[[nodiscard]] HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) const NABI_NOEXCEPT;

		XAudioObjects& m_XAudioObjects;
	};
} // namespace nabi::Audio
