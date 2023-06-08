#pragma once
#include "Core.h"

#include "CoreSingletonComponents\AudioStateComponent.h"

namespace nabi::Audio
{
	class AudioSourceVoice;
} // namespace nabi::Audio

namespace ecs::AudioModule
{
	struct PlaySettings final
	{
		std::optional<float> m_Volume;
	};
	PlaySettings constexpr c_DefaultPlaySettings =
	{
		.m_Volume = 1.0f
	};

	// --- Pool ---

	void InitSourceVoicePool(nabi::Context& context, size_t const poolSize);
	void DestroyAllEffects(nabi::Context& context);
	void DestroyAllVoices(nabi::Context& context);

	nabi::Audio::AudioSourceVoice* GetFirstReadySourceVoiceFromPool(nabi::Context& context);

	// --- Loading ---

	void LoadAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, std::string const& path, 
		nabi::Audio::AudioCommand::LoadSettings const& loadSettings);

	// --- Playing ---

	void PlayAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings);

	// --- State ---

	inline SComp::AudioStateComponent const& GetAudioStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::AudioStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Audio));
	}
	inline SComp::AudioStateComponent& GetAudioStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::AudioStateComponent&>(GetAudioStateComponent(const_cast<nabi::Context const&>(context)));
	}
} // namespace ecs::AudioModule
