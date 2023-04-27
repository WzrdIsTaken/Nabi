#pragma once
#include "Core.h"

namespace ecs
{
	class InputSystem final : public nabi::ECS::SystemBase
	{
	public:
		InputSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~InputSystem();

		void Update(/*TODO - game time?*/);

	private:
		void UpdateKeyboard() const;
		void UpdateMouse() const;
		void UpdateControllers() const;

		// Keyboard
		void OnKeyboardKeyDown(WPARAM const wParam, LPARAM const lParam) const;
		void OnKeyboardKeyUp(WPARAM const wParam, LPARAM const lParam) const;

		// Mouse
		void OnMouseLeftButtonDown(WPARAM const wParam, LPARAM const lParam) const;
		void OnMouseLeftButtonUp(WPARAM const wParam, LPARAM const lParam) const;
		void OnMouseRightButtonDown(WPARAM const wParam, LPARAM const lParam) const;
		void OnMouseRightButtonUp(WPARAM const wParam, LPARAM const lParam) const;
		void OnMouseMove(WPARAM const wParam, LPARAM const lParam) const;

		REFLECT_PRIVATES(InputSystem)
	};
} // namespace ecs