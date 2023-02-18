#pragma once
#include "Core.h"

namespace ecs
{
	class RenderSystem final : public nabi::ECS::SystemBase
	{
	public:
		using SystemBase::SystemBase;

		void Render(/*TODO - game time?*/);

	private:
		REFLECT_PRIVATES(RenderSystem)
	};
} // namespace nabi
