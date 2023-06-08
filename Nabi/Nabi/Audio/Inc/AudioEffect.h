#pragma once
#include "EngineCore.h"
#include "AudioCore.h"

namespace nabi::Audio
{
	struct AudioEffect final
	{
		std::shared_ptr<BYTE[]> m_DataBuffer;
		WAVEFORMATEXTENSIBLE m_WFX;
		XAUDIO2_BUFFER m_Buffer;

		AudioEffect() NABI_NOEXCEPT
			: m_DataBuffer{ nullptr }
			, m_WFX{}
			, m_Buffer{}
		{
		}
	};
} // namespace nabi::Audio
