#pragma once

#include "entt.h"

#include "AudioCommand.h"
#include "EntityCreator.h"
#include "NabiEventsManager.h"
#include "RenderCommand.h"
#include "ThreadCommand.h"
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
			Audio,
			ENUM_COUNT
		};

		// Core
		void* const m_CorePointer = nullptr;
		std::unique_ptr<Window> m_Window;
		NabiEventsManager m_NabiEventsManager;

		// Entities
		entt::registry m_Registry;
		std::array<entt::entity, SingletonEntities::ENUM_COUNT> m_SingletonEntites;

		std::unique_ptr<ECS::EntityCreator> m_EntityCreator;

		// Commands
		std::unique_ptr<Rendering::RenderCommand> m_RenderCommand;
		std::unique_ptr<Audio::AudioCommand> m_AudioCommand;
		std::unique_ptr<Threading::ThreadCommand> m_ThreadCommand;
	};
} // namespace nabi
