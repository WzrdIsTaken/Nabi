#pragma once

#include "entt.h"

#include <any>

#include "EntityCreator.h"
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
			ENUM_COUNT
		};

		// Core
		std::unique_ptr<Window> m_Window;

		// Entities
		entt::registry m_Registry;
		std::array<entt::entity, SingletonEntities::ENUM_COUNT> m_SingletonEntites;

		std::unique_ptr<nabi::Reflection::EntityCreator> m_EntityCreator;

		// Graphic
		std::unique_ptr<Rendering::RenderCommand> m_RenderCommand;

		// Game
		std::unordered_map<int, std::any> m_GameResources; // An easy, global way to pass data between game systems if needed
	};
} // namespace nabi
