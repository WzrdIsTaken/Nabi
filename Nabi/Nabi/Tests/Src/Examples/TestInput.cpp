#include "TestCore.h"

#include "Examples\TestInput.h"

#include "CoreModules\InputModule.h"
#include "InputCodes.h"
#include "InputState.h" 

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	using namespace ecs::InputModule;
	using namespace nabi::Input;

	TestInput::TestInput(nabi::Context& context)
		: m_Context(context)
		, m_InputSystem(nullptr)
	{
	}

	bool TestInput::Init()
	{
		// TODAY
		// folder restructure (see phone note)
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "InputId"_hs, "GroupId"_hs);
		return true;
	}

	bool TestInput::Update()
	{
		m_InputSystem->Update();

		TestKeyboard();
		TestMouse();
		TestController();

		return true;
	}

	bool TestInput::Render()
	{
		return false;
	}

	void TestInput::TestKeyboard() const
	{
		InputState const aKeyInputState = GetKeyboardKey(m_Context, Keys::A);
		TestInputBase(aKeyInputState, "A");
	}

	void TestInput::TestMouse() const
	{
		InputState const leftMouseButtonInputState = GetMouseButton(m_Context, Mouse::LeftButton);
		TestInputBase(leftMouseButtonInputState, "Left Mouse");
	}

	void TestInput::TestController() const
	{
		InputState const aControllerButtonInputState = GetControllerButton(m_Context, Controller::MasterController, Controller::Buttons::A);
		TestInputBase(aControllerButtonInputState, "A (controller)");
	}

	void TestInput::TestInputBase(InputState const keyState, std::string const& keyName) const
	{
		std::string stateString = "";
		switch (keyState)
		{
		case InputState::Pressed:
			stateString = "pressed";
			break;
		case InputState::Held:
			stateString = "held";
			break;
		case InputState::Released:
			stateString = "released";
			break;
		case InputState::Up:
			// stateString = "up"; (spams the console xD)
			break;
		}

		if (!stateString.empty())
		{
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST << WRAP(keyName, "'") << " is " << stateString << "!" << ENDLINE);
		}
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
