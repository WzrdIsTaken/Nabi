#pragma once

#include "entt.h"

#include <any>

#include "EntityCreator.h"
#include "NabiEventsManager.h"
#include "RenderCommand.h"
#include "Window.h"

namespace nabi
{
	/// <summary>
	/// A collection references to core level systems. Initialized in NabiCore
	/// </summary>
	struct Context final
	{
		enum SingletonEntities : size_t
		{
			Graphic,
			Input,
			Physics,
			ENUM_COUNT
		};

		// Core
		std::unique_ptr<Window> m_Window;
		NabiEventsManager m_NabiEventsManager;

		// Entities
		entt::registry m_Registry;
		std::array<entt::entity, SingletonEntities::ENUM_COUNT> m_SingletonEntites;

		std::unique_ptr<nabi::ECS::EntityCreator> m_EntityCreator;

		// Graphic
		std::unique_ptr<Rendering::RenderCommand> m_RenderCommand;
	};
} // namespace nabi
