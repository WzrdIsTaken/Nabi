#include "TestCore.h"

#include "Examples\TestAudio.h"

#include "AudioSourceVoice.h"
#include "CoreComponents\AudioEmitterComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\AudioModule.h"
#include "CoreModules\InputModule.h"
#include "CoreSystems\AudioSystem.h"
#include "CoreSystems\InputSystem.h"
#include "WorldConstants.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	auto constexpr c_AudioID = static_cast<ecs::SComp::AudioStateComponent::AudioID>(TestAudio::AudioIDs::BingoBangoBongo);

	TestAudio::TestAudio(nabi::Context& context)
		: m_Context(context)
		, m_AudioEmitterEntity(entt::null)
	{
	}

	TestAudio::~TestAudio()
	{
		// These cause exit code 3 when called here, but not because of anything audio related. I think the registry is already 
		// destroyed by the time we get to this point, so trying to get the AudioStateComponent doesn't work. 
		// In the actual game code, find an appropriate place for these methods!
		//ecs::AudioModule::DestroyAllEffects(m_Context);
		//ecs::AudioModule::DestroyAllVoices(m_Context);
		// UPDATE 13/06/23 - I put these in ~NabiCore. The audio voice pool is also now created there as well

		// Think about how a StopAudioEffect function would be written
		// Gotta make it so that audio resources follow the same flow as others, ie with an asset bank (see todo.txt for this stuff)
		// Also gotta add in the reflection and stuff for the audio components + think about how the resource flow is going to work
		//	- Audio resource components should have a map structure of audio id - audio path i recon
	}

	bool TestAudio::Init()
	{
		// --- Systems ---

		m_AudioSystem = std::make_unique<ecs::AudioSystem>(m_Context, "Audio"_hs, "NabiAudioTestSystems"_hs);
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "Input"_hs, "NabiAudioTestSystems"_hs);

		// --- Load Audio ---

		using namespace nabi::Audio;

		std::string const path = "Tests/Data/Audio/bingo_bango_bongo_bish_bash_bosh.wav";
		AudioCommand::LoadSettings loadSettings; // = AudioCommand::c_DefaultLoadSettings
		loadSettings.m_Loop = false;

		//ecs::AudioModule::InitSourceVoicePool(m_Context, 10u, 10u);
		ecs::AudioModule::LoadAudioEffect(m_Context, c_AudioID, path, loadSettings);

		// --- Create an audio emitter component ---

		m_AudioEmitterEntity = m_Context.m_EntityCreator->CreateEntity();
		m_Context.m_Registry.emplace<ecs::AudioEmitterComponent>(m_AudioEmitterEntity);
		m_Context.m_Registry.emplace<ecs::TransformComponent>(m_AudioEmitterEntity);

		// --- Move the listener ---
		dx::XMFLOAT3 constexpr position = { 0.0f, 0.0f, 0.0f };
		dx::XMFLOAT3 constexpr rotation = { 0.0f, 0.0f, 0.0f };
		dx::XMFLOAT3 constexpr velocity = { 0.0f, 0.0f, 0.0f };
		m_Context.m_AudioCommand->SetListener(position, rotation, velocity);

		return true;
	}

	bool TestAudio::Update()
	{
		using namespace nabi::Input;
		InputState const wKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_W);
		InputState const eKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_E);

		if (wKeyState == InputState::Pressed)
		{
			ecs::AudioModule::Play2DAudioEffect(m_Context, c_AudioID, ecs::AudioModule::c_DefaultPlaySettings);
		}
		if (eKeyState == InputState::Pressed)
		{
			auto& audioEmitterComponent = m_Context.m_Registry.get<ecs::AudioEmitterComponent>(m_AudioEmitterEntity);
			ecs::AudioModule::Play3DAudioEffect(m_Context, audioEmitterComponent, c_AudioID, ecs::AudioModule::c_DefaultPlaySettings);
		}

		return true;
	}

	bool TestAudio::Render()
	{
		return false;
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
