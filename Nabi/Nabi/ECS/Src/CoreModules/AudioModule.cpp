#include "Core.h"

#include "CoreModules\AudioModule.h"

namespace ecs::AudioModule
{
	void InitSourceVoicePool(nabi::Context& context, size_t const poolSize)
	{
		ASSERT_CODE
		(
			static bool poolInitialized = false;

			ASSERT(!poolInitialized, "Initializing the audio source voice pool more than once");
			poolInitialized = true;
		)

		SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool = GetAudioStateComponent(context).m_SourceVoicePool;
		sourceVoicePool.resize(poolSize);
	}

	void DestroyAllEffects(nabi::Context& context)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;

		for (auto& [audioID, audioEffect] : audioEffects)
		{
			context.m_AudioCommand->DestroyAudioEffect(audioEffect);
		}
	}

	void DestroyAllVoices(nabi::Context& context)
	{
		SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool = GetAudioStateComponent(context).m_SourceVoicePool;

		for (auto& sourceVoice : sourceVoicePool)
		{
			context.m_AudioCommand->DestroyAudioSourceVoice(sourceVoice);
		}
	}

	nabi::Audio::AudioSourceVoice* GetFirstReadySourceVoiceFromPool(nabi::Context& context)
	{
		nabi::Audio::AudioSourceVoice* audioSourceVoice = nullptr;
		SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool = GetAudioStateComponent(context).m_SourceVoicePool;

		ASSERT(!sourceVoicePool.empty(), "Trying to get a source voice from the pool, but the pool is empty");

		for (auto& sourceVoice : sourceVoicePool)
		{
			if (sourceVoice.IsReady())
			{
				audioSourceVoice = &sourceVoice;
				break;
			}
		}

		ASSERT(audioSourceVoice, "No available audio source voices! The audio clip may not play on time, or not at all");
		// To fix this ^, just increase AudioStateComponent::m_SourceVoicePool's size (via the call to InitSourceVoicePool)

		return audioSourceVoice;
	}

	// 

	void LoadAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, std::string const& path, 
		nabi::Audio::AudioCommand::LoadSettings const& loadSettings)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;
		bool const effectAlreadyLoaded = audioEffects.find(audioID) != audioEffects.end();

		if (!effectAlreadyLoaded)
		{
			nabi::Audio::AudioEffect audioEffect;
			context.m_AudioCommand->LoadAudioEffect(audioEffect, path, loadSettings);

			audioEffects.emplace(audioID, std::move(audioEffect));
		}
		else
		{
			ASSERT_FAIL("Loading an audio effect with ID " << WRAP(audioID, "'") << " but its already loaded");
		}
	}

	// 

	void PlayAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;

		auto effect = audioEffects.find(audioID);
		if (effect != audioEffects.end())
		{
			nabi::Audio::AudioSourceVoice* const audioSourceVoice = GetFirstReadySourceVoiceFromPool(context);
			if (audioSourceVoice)
			{
				nabi::Audio::AudioEffect& audioEffect = effect->second;
				context.m_AudioCommand->LoadAudioVoice(audioEffect, *audioSourceVoice);

				IXAudio2SourceVoice* const sourceVoice = audioSourceVoice->GetSourceVoice();
				if (playSettings.m_Volume.has_value())
				{
					sourceVoice->SetVolume(playSettings.m_Volume.value());
				}
				// ...any other settings

				sourceVoice->Start();
			}
		}
		else
		{
			ASSERT_FAIL("Trying to play an audio effect with ID " << WRAP(audioID, "'") << " but no effect with that id is loaded");
		}
	}
} // namespace ecs::AudioModule
