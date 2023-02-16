#pragma once

#include "entt.h"
#include "RenderCommand.h"

namespace nabi
{
	/// <summary>
	/// A collection references to core level systems. Initialized in NabiCore
	/// </summary>
	struct Context
	{
		enum SingletonEntities : size_t
		{
			Graphic,
			Input,
			ENUM_COUNT
		};

		// Entities
		entt::registry m_Registry;
		std::array<entt::entity, SingletonEntities::ENUM_COUNT> m_SingletonEntites;

		// Graphic
		std::unique_ptr<Rendering::RenderCommand> m_RenderCommand;
		// Window? Need a way of registering events but I'm not sure if this is it
	};
} // namespace nabi
