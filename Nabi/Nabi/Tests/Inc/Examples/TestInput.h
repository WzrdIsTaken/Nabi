#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "CoreSystems\InputSystem.h"

#ifdef RUN_TESTS

namespace nabi::Input
{
	enum class InputState : int;
} // namespace nabi::Input

namespace nabitest::Examples
{
	class TestInput final : public IExample
	{
	public:
		TestInput(nabi::Context& context);

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		void TestKeyboard() const;
		void TestMouse() const;
		void TestController() const;
		void TestInputBase(nabi::Input::InputState const keyState, std::string const& keyName) const;

		nabi::Context& m_Context;
		std::unique_ptr<ecs::InputSystem> m_InputSystem; // making this a ptr is a hack to get around example init order problems
	};
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
