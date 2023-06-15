#include "Core.h"

#include "CoreModules\AudioModule.h"

#include "CoreComponents\AudioEmitterComponent.h"

namespace ecs::AudioModule
{
	namespace
	{
		void PlayAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, 
			PlaySettings const& playSettings, SComp::AudioStateComponent::SourceVoicePool& voicePool, 
			std::optional<std::function<void(nabi::Audio::AudioSourceVoice&)>> configureSourceVoice)
		{
			SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;

			auto effect = audioEffects.find(audioID);
			if (effect != audioEffects.end())
			{
				nabi::Audio::AudioSourceVoice* const audioSourceVoice = GetFirstReadySourceVoiceFromPool(context, voicePool);
				if (audioSourceVoice)
				{
					nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> const audioEffect = effect->second;
					if (audioEffect.IsValid())
					{
						context.m_AudioCommand->LoadAudioVoice(*audioEffect.GetResourceNonConst(), *audioSourceVoice);

						IXAudio2SourceVoice* const sourceVoice = audioSourceVoice->GetSourceVoice();
						if (configureSourceVoice)
						{
							(*configureSourceVoice)(*audioSourceVoice);
						}

						if (playSettings.m_Volume.has_value())
						{
							sourceVoice->SetVolume(playSettings.m_Volume.value());
						}
						// ...any other settings

						sourceVoice->Start();
					}
					else
					{
						ASSERT_FAIL("Trying to play an audio effect with ID " << WRAP(audioID, "'") << "but its null! Did the resource get removed a resource bank?");
					}
				}
			}
			else
			{
				ASSERT_FAIL("Trying to play an audio effect with ID " << WRAP(audioID, "'") << " but no effect with that id is loaded");
			}
		}
	}

	void InitSourceVoicePool(nabi::Context& context, size_t const poolSize2D, size_t const poolSize3D)
	{
		ASSERT_CODE
		(
			static bool poolInitialized = false;

			ASSERT(!poolInitialized, "Initializing the audio source voice pool more than once");
			poolInitialized = true;
		)

		GetAudioStateComponent(context).m_2DSourceVoicePool.resize(poolSize2D);
		GetAudioStateComponent(context).m_3DSourceVoicePool.resize(poolSize3D);
	}

	void DestroyAllEffects(nabi::Context& context)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;

		for (auto& [audioID, audioEffect] : audioEffects)
		{
			if (audioEffect.IsValid())
			{
				context.m_AudioCommand->DestroyAudioEffect(*audioEffect.GetResourceNonConst());
			}
			else
			{
				LOG(LOG_PREP, LOG_ERROR, LOG_CATEGORY_AUDIO <<
					"Tried to destroy an audio effect, but the audio effect was null. This may result in a bad shutdown when xaudio terminates" << ENDLINE);
			}
		}
	}

	void DestroyAllVoices(nabi::Context& context)
	{
		auto destroyAllVoicesHelper =
			[&context](SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool) -> void
			{
				for (auto& sourceVoice : sourceVoicePool)
				{
					context.m_AudioCommand->DestroyAudioVoice(sourceVoice);
				}
			};

		destroyAllVoicesHelper(GetAudioStateComponent(context).m_2DSourceVoicePool);
		destroyAllVoicesHelper(GetAudioStateComponent(context).m_3DSourceVoicePool);
	}

	nabi::Audio::AudioSourceVoice* GetFirstReadySourceVoiceFromPool(nabi::Context& /*context*/, SComp::AudioStateComponent::SourceVoicePool& sourceVoicePool)
	{
		nabi::Audio::AudioSourceVoice* audioSourceVoice = nullptr;

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

	void MapLoadedAudioEffectToID(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, nabi::Resource::ResourceRef<nabi::Audio::AudioEffect> const audioEffect)
	{
		SComp::AudioStateComponent::AudioEffectLookup& audioEffects = GetAudioStateComponent(context).m_AudioEffects;
		if (!audioEffects.contains(audioID))
		{
			audioEffects.emplace(audioID, audioEffect);
		}
	}

	// 

	void Play2DAudioEffect(nabi::Context& context, SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings)
	{
		PlayAudioEffect(context, audioID, playSettings, GetAudioStateComponent(context).m_2DSourceVoicePool, std::nullopt /*no custom play behaviour required*/);
	}

	void Play3DAudioEffect(nabi::Context& context, AudioEmitterComponent& audioEmitterComponent, 
		SComp::AudioStateComponent::AudioID const audioID, PlaySettings const& playSettings)
	{
		PlayAudioEffect(context, audioID, playSettings, GetAudioStateComponent(context).m_3DSourceVoicePool,
			[&context, &audioEmitterComponent](nabi::Audio::AudioSourceVoice& audioSourceVoice) -> void
			{
				context.m_AudioCommand->Calculate3DSoundProperties(
					audioEmitterComponent.m_Emitter,
					audioSourceVoice,
					audioEmitterComponent.m_DSPSettings
				);
			});
	}
} // namespace ecs::AudioModule

/*
* This logic is now handled by a special ResourceLoader, it is deprecated
* 
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
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_AUDIO << "Loading an audio effect with ID " << WRAP(audioID, "'") << " but its already loaded" << ENDLINE);
		}
	}
*/
