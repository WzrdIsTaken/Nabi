#pragma once
#include "EngineCore.h"
#include "AudioCore.h"

#include "AudioCommand.h"

namespace nabi
{
	struct Context;
} // namespace nabi

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

	class AudioEffectLoader final
	{
	public:
		typedef std::shared_ptr<AudioEffect> ResourceType;

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
		void SetLoadSettings(AudioCommand::LoadSettings const& loadSettings) NABI_NOEXCEPT;

	private:
		AudioCommand::LoadSettings m_LoadSettings = AudioCommand::c_DefaultLoadSettings;
	};
} // namespace nabi::Audio
