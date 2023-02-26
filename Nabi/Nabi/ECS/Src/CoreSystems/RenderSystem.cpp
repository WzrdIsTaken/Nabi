#include "Core.h"

#include "CoreSystems\RenderSystem.h"

#include "CoreComponents\CameraComponent.h"
#include "CoreComponents\EntityInfoComponent.h"
#include "CoreComponents\GraphicsComponent.h"
#include "CoreComponents\ModelComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "DirectXUtils.h"
#include "ResourceWrappers.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(RenderSystem)
	RELFECT_SYSTEM_END(RenderSystem)

	void RenderSystem::Render()
	{
		// Get the graphics entity. This stores the camera, constant buffers, etc
		entt::entity graphicEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Graphic);

		// Cache the graphics component
		GraphicsComponent& graphicsComponent = m_Context.m_Registry.get<GraphicsComponent>(graphicEntity);

		// Update the per frame constant buffer
		{
			// Get the constant buffer
			nabi::Rendering::ConstantBuffer const perFrameConstantBuffer 
				= graphicsComponent.m_ConstantBuffers.at(nabi::Rendering::ConstantBufferIndex::PerFrame);
			nabi::Rendering::PerFrame perFrameConstantBufferData;

			// Get the data
			CameraComponent const& camera = m_Context.m_Registry.get<CameraComponent>(graphicEntity);
			dx::XMMATRIX viewMatrix = dx::XMLoadFloat4x4(&camera.m_View);
			dx::XMMATRIX projectionMatrix = dx::XMLoadFloat4x4(&camera.m_Projection);

			// Transpose the matrixs for HLSL
			viewMatrix = dx::XMMatrixTranspose(viewMatrix);
			projectionMatrix = dx::XMMatrixTranspose(projectionMatrix);

			// Store the matrix data in the constant buffer
			dx::XMStoreFloat4x4(&perFrameConstantBufferData.m_ViewMatrix, viewMatrix);
			dx::XMStoreFloat4x4(&perFrameConstantBufferData.m_ProjectionMatrix, projectionMatrix);
			perFrameConstantBufferData.m_CameraPosition = camera.m_Position;

			// Update the constant buffer
			m_Context.m_RenderCommand->UpdateConstantBuffer(perFrameConstantBuffer, &perFrameConstantBufferData);

			// Cache the values for debug
#ifdef USE_DEBUG_UTILS
			m_DebugProjectionMatrix = projectionMatrix;
			m_DebugViewMatrix = viewMatrix;
#endif // USE_DEBUG_UTILS
		}

		// Cache the per mesh contant buffer 
		nabi::Rendering::ConstantBuffer const perMeshConstantBuffer
			= graphicsComponent.m_ConstantBuffers.at(nabi::Rendering::ConstantBufferIndex::PerMesh);
		nabi::Rendering::PerMesh perMeshConstantBufferData;

		// Loop through all the models
		m_Context.m_Registry.view<TransformComponent, MeshComponent, ShaderComponent, TextureComponent>()
			.each([&](
#ifdef USE_DEBUG_UTILS	
				entt::entity const entity,
#endif // USE_DEBUG_UTILS
				auto& transformComponent, auto const& meshComponent, auto const& shaderComponent, auto const& textureComponent)
				{
					// Update the per mesh constant buffer
					{
						// Get the data
						dx::XMVECTOR const transformVector = dx::XMLoadFloat3(&transformComponent.m_Position);
						dx::XMVECTOR const scaleVector = dx::XMLoadFloat3(&transformComponent.m_Scale);

						dx::XMVECTOR const rotationVector = dx::XMLoadFloat3(&transformComponent.m_Rotation);
						dx::XMVECTOR const rotationQuaternion = dx::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

						// Calculate the model matrix
						dx::XMMATRIX modelMatrix = dx::XMMatrixAffineTransformation(scaleVector, dx::XMVectorZero(), rotationQuaternion, transformVector);

						// Transpose for HLSL
						modelMatrix = dx::XMMatrixTranspose(modelMatrix);

						// Store the matrix data in the constant buffer
						dx::XMStoreFloat4x4(&perMeshConstantBufferData.m_ModelMatrix, modelMatrix);

						// Update the constant buffer
						m_Context.m_RenderCommand->UpdateConstantBuffer(perMeshConstantBuffer, &perMeshConstantBufferData);

						// Lets go dude!
						transformComponent.m_Rotation.x += 0.1f;
						transformComponent.m_Rotation.y += 0.1f;
						transformComponent.m_Rotation.z += 0.1f;

						// Debug output
#ifdef USE_DEBUG_UTILS
						m_DebugModelMatrix = modelMatrix;
						DebugTraceOutput(entity);
#endif // USE_DEBUG_UTILS
					}

					using namespace nabi::Rendering;
					using namespace nabi::Resource;

					// Get the model data
					std::shared_ptr<Mesh const> const meshResource = meshComponent.m_MeshResource.GetResource();
					IndexBuffer const& indexBuffer = meshResource->m_IndexBuffer;
					VertexBuffer const& vertexBuffer = meshResource->m_VertexBuffer;
					UINT const triangleCount = meshResource->m_Triangles.size();

					// Get the shader data
					std::shared_ptr<PixelShader const> const pixelShaderResource = shaderComponent.m_PixelShaderResource.GetResource();
					std::shared_ptr<VertexShader const> const vertexShaderResource = shaderComponent.m_VertexShaderResource.GetResource();

					// Get the texture data
					std::shared_ptr<Texture const> const textureResource = textureComponent.m_TextureResource.GetResource();

					// Bind the resources to the pipeline
					m_Context.m_RenderCommand->BindIndexBuffer(indexBuffer);
					m_Context.m_RenderCommand->BindVertexBuffer(vertexBuffer);

					m_Context.m_RenderCommand->BindPixelShader(*pixelShaderResource);
					m_Context.m_RenderCommand->BindVertexShader(*vertexShaderResource);
					m_Context.m_RenderCommand->BindTexture(*textureResource);

					// Draw!
					m_Context.m_RenderCommand->DrawIndexed(triangleCount);
				});
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
			"[Rendering] The model on entity " << WRAP(entityName, "'") << " has a: "     << NEWLINE
			"Model Matrix of          : " << MatrixToString(m_DebugModelMatrix)           << NEWLINE
			"View Matrix of           : " << MatrixToString(m_DebugViewMatrix)            << NEWLINE
			"Projection Matrix of     : " << MatrixToString(m_DebugProjectionMatrix)      << NEWLINE
			"And a resultant matrix of: " << MatrixToString(result);

		// Log
		LOG(LOG_PREP, LOG_TRACE, logMessage.str() << ENDLINE);
	}
#endif // USE_DEBUG_UTILS
} // namespace nabi
