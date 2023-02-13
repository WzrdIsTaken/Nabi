#pragma once

#include "entt.h"

namespace nabi
{
	/// <summary>
	/// A collection references to core level systems
	/// </summary>
	struct Context
	{
		/*
		enum SingletonEntities
		{
			Graphic,
			Input,
			ENUM_COUNT
		};

		Context(entt::registry& registery, RenderCommand& renderCommand)
			: m_Registry(registery)
			, m_RenderCommand(renderCommand)
			, m_SingletonEntites{}
		{
			m_SingletonEntites.fill(entt::null);
		}

		// Entities
		entt::registry& m_Registry;
		std::array<entt::entity, SingletonEntities::ENUM_COUNT> m_SingletonEntites;

		// Graphic
		RenderCommand& m_RenderCommand;
		// Window?
		*/
	};
} // namespace nabi
