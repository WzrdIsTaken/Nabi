#include "TestCore.h"

#include "Examples\TestInput.h"

#include "pugixml.hpp"

#include "CoreComponents\EntityInfoComponent.h"
#include "CoreComponents\UIComponents.h"
#include "CoreModules\InputModule.h"
#include "CoreModules\UIModule.h"
#include "InputCodes.h"
#include "InputState.h" 
#include "XmlParser.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	REFLECT_UI_SCENE_BEGIN(TestScene)
		REFLECT_UI_SCENE_METHOD(nabitest::Examples::TestButtonResponseOne, "TestButtonResponseOne")
		REFLECT_UI_SCENE_METHOD(nabitest::Examples::TestButtonResponseTwo, "TestButtonResponseTwo")
	REFLECT_UI_SCENE_END(TestScene)

	using namespace ecs::InputModule;
	using namespace nabi::Input;

	TestInput::TestInput(nabi::Context& context)
		: m_Context(context)
		, m_InputSystem(nullptr)
		, m_UISystem(nullptr)
	{
	}

	bool TestInput::Init()
	{
		// --- Systems ---
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "InputId"_hs, "GroupId"_hs);
		m_UISystem = std::make_unique<ecs::UISystem>(m_Context, "UIId"_hs, "GroupId"_hs);

		// --- Entities ---

		TestXmlEntities();

		return true;
	}

	bool TestInput::Update()
	{
		m_InputSystem->Update();
		m_UISystem->Update();

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
		InputState const aKeyInputState = GetKeyboardKey(m_Context, InputCode::Key_A);
		TestInputBase(aKeyInputState, "A");
	}

	void TestInput::TestMouse() const
	{
		{
			dx::XMFLOAT2 constexpr mockElementPosition = { 0.0f, 0.0f };
			dx::XMFLOAT2 constexpr mockElementDims = { 100.0f, 100.0f };
		
			InputState const leftMouseButtonInputState = GetMouseButton(m_Context, InputCode::Mouse_LeftButton);
			if (leftMouseButtonInputState == InputState::Pressed)
			{
				bool const clickInsideElement = ecs::UIModule::CheckIfMouseIsOverElement(m_Context, mockElementPosition, mockElementDims);
				if (clickInsideElement)
				{
					LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST << "Click inside mock element!" << ENDLINE);
				}
			}
		}

		InputState const leftMouseButtonInputState = GetMouseButton(m_Context, InputCode::Mouse_LeftButton);
		TestInputBase(leftMouseButtonInputState, "Left Mouse");
	}

	void TestInput::TestController() const
	{
		InputState const aControllerButtonInputState = GetControllerButton(m_Context, Controller::MasterController, InputCode::Controller_A);
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

	void TestInput::TestHardcodedEntities()
	{
		// Button
		ecs::ButtonComponent buttonComponent;
		buttonComponent.m_Interactable = true;
		buttonComponent.m_Active = true;
		buttonComponent.m_Selected = false;

		buttonComponent.m_InputType = InputType::Keyboard;
		buttonComponent.m_ActivationKey = InputCode::Key_G;
		buttonComponent.m_AltInputType = InputType::Keyboard;
		buttonComponent.m_AltActivationKey = InputCode::Key_H;

		buttonComponent.m_Scene = entt::hashed_string("TestScene");
		buttonComponent.m_Action = entt::hashed_string("TestButtonResponseOne");

		nabi::Reflection::EntityCreator::EntityCreationSettings settings;
		settings.m_EntityName = "ButtonEntity";
		settings.m_AppendUUID = false;

		entt::entity const uiEntity = m_Context.m_EntityCreator->CreateEntity(&settings);
		m_Context.m_Registry.emplace<ecs::ButtonComponent>(uiEntity, buttonComponent);

		// Scene 
		ecs::UISceneComponent uiScene;
		uiScene.m_SceneName = entt::hashed_string("TestScene");
		uiScene.m_UISceneEntityNames = "ButtonEntity"_hs; // This can be a list - like 'Button1, Button2' etc

		entt::entity uiSceneEntity = m_Context.m_EntityCreator->CreateEntity();
		m_Context.m_Registry.emplace<ecs::UISceneComponent>(uiSceneEntity, uiScene);
	}

	void TestInput::TestXmlEntities()
	{
		nabi::Reflection::XmlParser xmlParser{};
		std::string const docPath = "Tests/Data/Input/test_ui_entities.xml";

		pugi::xml_document const doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, m_Context.m_Registry);
	}

	void TestButtonResponseOne(nabi::Context& /*context*/, entt::entity const /*uiEntity*/)
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST << "Button one was clicked!" << ENDLINE);
	}

	void TestButtonResponseTwo(nabi::Context& context, entt::entity const /*uiEntity*/)
	{
		//context.m_Registry.destroy(entt::entity(7));
		//context.m_Registry.destroy(entt::entity(4));

		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST << "Button two was clicked!" << ENDLINE);
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
