#include "Core.h"

#include "CoreSystems\RenderSystem.h"

#include "CoreComponents\CameraComponent.h"
#include "CoreComponents\GraphicsComponent.h"
#include "CoreComponents\ModelComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "Resource.h"

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
		}

		// Cache the per mesh contant buffer 
		nabi::Rendering::ConstantBuffer const perMeshConstantBuffer
			= graphicsComponent.m_ConstantBuffers.at(nabi::Rendering::ConstantBufferIndex::PerMesh);
		nabi::Rendering::PerMesh perMeshConstantBufferData;

		// Loop through all the models
		m_Context.m_Registry.view<TransformComponent, MeshComponent, ShaderComponent, TextureComponent>()
			.each([&](auto& transformComponent, auto& meshComponent, auto& shaderComponent, auto& textureComponent)
				{
					// Update the per mesh constant buffer
					{
						// Get the data
						dx::XMVECTOR const transformVector = dx::XMLoadFloat3(&transformComponent.m_Position);
						dx::XMVECTOR const rotationVector = dx::XMLoadFloat3(&transformComponent.m_Scale /*, 1?*/);
						dx::XMVECTOR const scaleVector = dx::XMLoadFloat3(&transformComponent.m_Rotation /*, 1?*/);

						// Calculate the model matrix
						dx::XMMATRIX model = dx::XMMatrixAffineTransformation(scaleVector, dx::XMVectorZero(), rotationVector, transformVector);

						// Transpose for HLSL
						model = dx::XMMatrixTranspose(model);

						// Store the matrix data in the constant buffer
						dx::XMStoreFloat4x4(&perMeshConstantBufferData.m_ModelMatrix, model);

						// Update the constant buffer
						m_Context.m_RenderCommand->UpdateConstantBuffer(perMeshConstantBuffer, &perMeshConstantBufferData);
					}

					/*
					using namespace nabi::Rendering;
					using namespace nabi::Resource;

					// Get the model data
					std::shared_ptr<Mesh const> const meshResource = meshComponent.m_MeshResource.GetResource();
					IndexBuffer const indexBuffer = {}; // meshResource.
					VertexBuffer const vertexBuffer = {}; // meshResource.

					// Get the shader data
					std::shared_ptr<PixelShader const> const pixelShaderResource = shaderComponent.m_PixelShaderResource.GetResource();
					std::shared_ptr<VertexShader const> const vertexShaderResource = shaderComponent.m_VertexShaderResource.GetResource();

					// Get the texture data
					std::shared_ptr<Texture const> const textureResource = textureComponent.m_TextureResource.GetResource();

					// Bind the resources to the pipeline
					m_Context.m_RenderCommand->BindIndexBuffer(indexBuffer);
					m_Context.m_RenderCommand->BindVertexBuffer(vertexBuffer);
					m_Context.m_RenderCommand->BindPixelShader(&pixelShaderResource);
					m_Context.m_RenderCommand->BindVertexShader(&vertexShaderResource);
					m_Context.m_RenderCommand->BindTexture(&textureResource);

					// Draw!
					//m_Context.m_RenderCommand->DrawIndexed
					*/
				});
	}
} // namespace nabi
