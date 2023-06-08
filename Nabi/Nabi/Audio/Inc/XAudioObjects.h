#pragma once
#include "EngineCore.h"
#include "WinCore.h"
#include "DirectXCore.h"
#include "AudioCore.h"

namespace nabi::Audio
{
	struct XAudioObjects final
	{
		wrl::ComPtr<IXAudio2> m_XAudio2;
		IXAudio2MasteringVoice* m_MasteringVoice;

		X3DAUDIO_HANDLE m_X3DAudio;
		X3DAUDIO_LISTENER m_Listener;
		XAUDIO2_VOICE_DETAILS m_MasteringVoiceDetails;
	};
	
	XAudioObjects const c_XAudioObjectsDefaultSettings
	{
		.m_XAudio2 = nullptr,
		.m_MasteringVoice = nullptr,
		.m_X3DAudio = NULL,
		.m_Listener = {},
		.m_MasteringVoiceDetails = {}
	};
} // namespace nabi::Audio
