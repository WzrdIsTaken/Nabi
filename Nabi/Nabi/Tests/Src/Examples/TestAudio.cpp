#include "TestCore.h"

#include "Examples\TestAudio.h"

#include "AudioSourceVoice.h"
#include "CoreModules\AudioModule.h"
#include "CoreModules\InputModule.h"
#include "CoreSystems\InputSystem.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	auto constexpr c_AudioID = static_cast<ecs::SComp::AudioStateComponent::AudioID>(TestAudio::AudioIDs::BingoBangoBongo);

	TestAudio::TestAudio(nabi::Context& context)
		: m_Context(context)
	{
		// Keep working on audio!
		// See Sam's code + notebook. I think I know what to do.

		// Need to ask about 2d audio though.. or perhaps can figure this out
	}

	TestAudio::~TestAudio()
	{
		// These cause exit code 3 when called here, but not because of anything audio related. I think the registry is already 
		// destroyed by the time we get to this point, so trying to get the AudioStateComponent doesn't work. 
		// In the actual game code, find an appropriate place for these methods!
		//ecs::AudioModule::DestroyAllEffects(m_Context);
		//ecs::AudioModule::DestroyAllVoices(m_Context);

		// Then 2D audio / moving the listener
		// Gotta write a core audio system
		// Think about how a StopAudioEffect function would be written

		// Then read through all code, make sure it makes sense / corrections, and then commit. Probs do this before the 2 points above
		
		// Thurs
		// - Have you done chores?
		// - Read through code, check its sound and commit. Then do points above
	}

	bool TestAudio::Init()
	{
		// --- Systems ---

		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "Input"_hs, "NabiAudioTestSystems"_hs);

		// --- Load Audio ---

		using namespace nabi::Audio;

		std::string const path = "Tests/Data/Audio/bingo_bango_bongo_bish_bash_bosh.wav";
		AudioCommand::LoadSettings loadSettings; // = AudioCommand::c_DefaultLoadSettings
		loadSettings.m_Loop = false;

		ecs::AudioModule::InitSourceVoicePool(m_Context, 10u);
		ecs::AudioModule::LoadAudioEffect(m_Context, c_AudioID, path, loadSettings);
		
		return true;
	}

	bool TestAudio::Update()
	{
		using namespace nabi::Input;
		InputState const wKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_W);

		if (wKeyState == InputState::Pressed)
		{
			ecs::AudioModule::PlayAudioEffect(m_Context, c_AudioID, ecs::AudioModule::c_DefaultPlaySettings);
		}

		return true;
	}

	bool TestAudio::Render()
	{
		return false;
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
