#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

#include "Buffers\IndexBuffer.h"
#include "Buffers\VertexBuffer.h"

namespace nabi
{
	struct Context;
}

namespace nabi::Rendering
{
	struct Mesh
	{
		IndexBuffer/*&?*/ m_IndexBuffer;
		VertexBuffer/*&?*/ m_VertexBuffer;

		std::vector<dx::XMFLOAT3> m_Vertices;
		std::vector<dx::XMFLOAT3> m_Normals;
		std::vector<dx::XMFLOAT2> m_Uvs;
		std::vector<UINT> m_Triangles;
	};

	class MeshLoader final
	{
	public:
		typedef std::shared_ptr<Mesh> ResourceType;

		ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
