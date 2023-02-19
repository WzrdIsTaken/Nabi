#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

#include "Buffers\IndexBuffer.h"
#include "Buffers\VertexBuffer.h"

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
} // namespace nabi::Rendering
