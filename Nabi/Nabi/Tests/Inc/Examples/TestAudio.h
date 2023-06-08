#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "CoreSingletonComponents\AudioStateComponent.h"

#ifdef RUN_TESTS

namespace ecs
{
	class InputSystem;
} // namespace ecs

namespace nabitest::Examples
{
	class TestAudio final : public IExample
	{
	public:
		enum class AudioIDs : ecs::SComp::AudioStateComponent::AudioID
		{
			BingoBangoBongo
		};

		TestAudio(nabi::Context& context);
		~TestAudio();

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		nabi::Context& m_Context;

		// Audio System 
		std::unique_ptr<ecs::InputSystem> m_InputSystem;
	};
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
