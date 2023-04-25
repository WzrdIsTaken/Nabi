#include "Core.h"

#include "CoreModules\UIModule.h"

#include "CoreModules\InputModule.h"

namespace ecs::UIModule
{
	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions)
	{
		dx::XMFLOAT2 const mousePosition = InputModule::GetMousePosition(context);
		return CheckIfMouseIsOverElement(context, mousePosition, elementPosition, elementDimensions);
	}

	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const mousePosition, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions)
	{
		bool mouseOverElement = true;

		mouseOverElement &= mousePosition.x > elementPosition.x;
		mouseOverElement &= mousePosition.y > elementPosition.y;
		mouseOverElement &= mousePosition.x < elementPosition.x + elementDimensions.x;
		mouseOverElement &= mousePosition.y < elementPosition.y + elementDimensions.y;

		return mouseOverElement;
	}
} // namespace ecs::UIModule
