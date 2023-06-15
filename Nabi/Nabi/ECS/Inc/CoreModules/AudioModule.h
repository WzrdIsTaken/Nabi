#pragma once
#include "Core.h"

#include "CoreSingletonComponents\AudioStateComponent.h"

namespace ecs
{
	struct AudioEmitterComponent;
} // namespace ecs

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

	void InitSourceVoicePool(nabi::Context& context, size_t const poolSize2D, size_t const poolSize3D);
	void DestroyAllEffects(nabi::Context& context);
	void DestroyAllVoices(nabi::Context& context);

	nabi::Audio::AudioSourceVoice* GetFirstReadySourceVoiceFromPool(nabi::Context& context, SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool);

	// --- Loading ---

	void MapLoadedAudioEffectToID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> const audioEffect);

	// --- Playing ---

	void Play2DAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings);
	void Play3DAudioEffect(nabi::Context& context, AudioEmitterComponent& audioEmitterComponent,
		SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings);

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
