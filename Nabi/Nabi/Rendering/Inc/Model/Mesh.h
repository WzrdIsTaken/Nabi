#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

#include "Buffers\IndexBuffer.h"
#include "Buffers\VertexBuffer.h"

namespace nabi
{
	struct Context;
} // namespace nabi
namespace objl
{
	struct Vector2;
	struct Vector3;
} // namespace objl

namespace nabi::Rendering
{
	struct Mesh final
	{
		IndexBuffer m_IndexBuffer;
		VertexBuffer m_VertexBuffer;
	};

	struct MeshData final
	{
		std::vector<dx::XMFLOAT3> m_Vertices;
		std::vector<dx::XMFLOAT3> m_Normals;
		std::vector<dx::XMFLOAT2> m_Uvs;
		std::vector<UINT> m_Triangles;
	};

	class MeshLoader final
	{
	public:
		typedef std::shared_ptr<Mesh> ResourceType;

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;

	private:
		[[nodiscard]] dx::XMFLOAT2 ObjlVector2ToDxFloat2(objl::Vector2 const& vector2) const NABI_NOEXCEPT;
		[[nodiscard]] dx::XMFLOAT3 ObjlVector3ToDxFloat3(objl::Vector3 const& vector3) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
