#pragma once
#include "Core.h"

#include "AudioEffect.h"
#include "AudioSourceVoice.h"

namespace ecs::SComp
{
	struct AudioStateComponent final : public nabi::ECS::ComponentBase
	{
		typedef uint32_t AudioID; // The enum which holds sounds should be of this type, and reflected with REFLECT_ENUM_UNDERLYING
		typedef std::unordered_map<AudioID, nabi::Audio::AudioEffect> AudioEffectLookup;
		typedef std::vector<nabi::Audio::AudioSourceVoice> SourceVoicePool;

		AudioEffectLookup m_AudioEffects;
		SourceVoicePool m_SourceVoicePool;

		AudioStateComponent()
			: m_AudioEffects{}
			, m_SourceVoicePool{}
		{
		}
	};
} // namespace ecs::SComp
