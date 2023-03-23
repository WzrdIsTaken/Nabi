#include "Core.h"

#include "CoreSystems\RenderSystem.h"

#include "CoreComponents\EntityInfoComponent.h"
#include "DirectXUtils.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(RenderSystem)
	RELFECT_SYSTEM_END(RenderSystem)

	void RenderSystem::Render()
	{
		// Get the graphics entity. This stores the camera, constant buffers, etc
		entt::entity graphicEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic);

		// Cache the graphics and camera component
		GraphicsComponent& graphicsComponent = m_Context.m_Registry.get<GraphicsComponent>(graphicEntity);
		CameraGroupComponent const& cameraGroupComponent = m_Context.m_Registry.get<CameraGroupComponent>(graphicEntity);

		// Complete render passes
		RenderPerspective(graphicsComponent, cameraGroupComponent);
		RenderOrthographic(graphicsComponent, cameraGroupComponent);
	}

	void RenderSystem::RenderPerspective(GraphicsComponent& graphicsComponent, CameraGroupComponent const& cameraGroupComponent)
	{
		CameraIndex::Enum constexpr camera = CameraIndex::Perspective;
		RenderInternal<Tags::DrawPerspective>(graphicsComponent, cameraGroupComponent, camera);
	}

	void RenderSystem::RenderOrthographic(GraphicsComponent& graphicsComponent, CameraGroupComponent const& cameraGroupComponent)
	{
		CameraIndex::Enum constexpr camera = CameraIndex::Orthographic;
		RenderInternal<Tags::DrawOrthographic>(graphicsComponent, cameraGroupComponent, camera);
	}

#ifdef USE_DEBUG_UTILS
	void RenderSystem::DebugTraceOutput(entt::entity const entity) const
	{
		// Work out what the resultant matrix should be
		dx::XMMATRIX result;
		result = dx::XMMatrixMultiply(m_DebugModelMatrix, m_DebugViewMatrix);
		result = dx::XMMatrixMultiply(result, m_DebugProjectionMatrix);

		// Find the entities name for logging
		std::string entityName = "";
		if (ecs::EntityInfoComponent* entityInfoComponent = m_Context.m_Registry.try_get<ecs::EntityInfoComponent>(entity))
		{
			entityName = entityInfoComponent->m_EntityName.data();
		}

		// Build the log message
		using namespace nabi::Utils::DirectXUtils;
		std::ostringstream logMessage;

		logMessage <<
			LOG_CATEGORY_RENDERING << "The model on entity " << WRAP(entityName, "'") << " has a: " << NEWLINE
			"Model Matrix of          : " << MatrixToString(m_DebugModelMatrix)                     << NEWLINE
			"View Matrix of           : " << MatrixToString(m_DebugViewMatrix)                      << NEWLINE
			"Projection Matrix of     : " << MatrixToString(m_DebugProjectionMatrix)                << NEWLINE
			"And a resultant matrix of: " << MatrixToString(result);

		// Log
		LOG(LOG_PREP, LOG_TRACE, logMessage.str() << ENDLINE);
	}
#endif // USE_DEBUG_UTILS
} // namespace ecs
