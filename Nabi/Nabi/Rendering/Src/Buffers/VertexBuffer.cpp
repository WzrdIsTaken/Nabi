#include "EngineCore.h"
#include "WinCore.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\VertexBuffer.h"

#include "Context.h"
#include "Model\Mesh.h"
#include "Model\Vertex.h"

namespace nabi::Rendering
{
	VertexBuffer nabi::Rendering::VertexBufferLoader::operator()(Mesh const& mesh, nabi::Context const& context) const NABI_NOEXCEPT
	{
		std::vector<Vertex> vertices;
		size_t const uvSize = mesh.m_Uvs.size();
		size_t const normalSize = mesh.m_Normals.size();

		for (size_t i = 0; i < mesh.m_Vertices.size(); ++i)
		{
			Vertex vertex;
			vertex.m_Position = mesh.m_Vertices[i];
			vertex.m_UV       = i < uvSize ? mesh.m_Uvs[i] : dx::XMFLOAT2();
			vertex.m_Normal   = i < normalSize ? mesh.m_Normals[i] : dx::XMFLOAT3();

			vertices.emplace_back(vertex);
		}

		VertexBuffer const vertexBuffer = context.m_RenderCommand->CreateVertexBuffer(vertices);
		return vertexBuffer;
	}
} // namespace nabi::Rendering