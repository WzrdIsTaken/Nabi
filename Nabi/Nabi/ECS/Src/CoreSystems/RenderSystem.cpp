#include "Core.h"

#include "CoreSystems\RenderSystem.h"

#include "CoreComponents\ModelComponent.h"
#include "CoreComponents\TransformComponent.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(RenderSystem)
	RELFECT_SYSTEM_END(RenderSystem)

	void RenderSystem::Render()
	{
		// Get the graphics entity. This stores the camera, constant buffers, etc
		entt::entity graphicEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic);

		// Update the per frame constant buffer

		// Cache the per mesh contant buffer 

		// Loop through all the models
		m_Context.m_Registry.view<TransformComponent, MeshComponent, ShaderComponent, TextureComponent>()
			.each([&](auto& transformComponent, auto& meshComponent, auto& shaderComponent, auto& textureComponent)
				{
					// Update the per mesh constant buffer
					{

					}

					// Get the model data

					// Get the shader data

					// Get the texture data

					// Bind the resources to the pipeline

					// Draw!
				});
	}
} // namespace nabi
