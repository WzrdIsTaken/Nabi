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
	struct RenderBuffers final
	{
		IndexBuffer m_IndexBuffer;
		VertexBuffer m_VertexBuffer;
	};
	typedef RenderBuffers Mesh;
	typedef RenderBuffers Sprite;

	struct MeshData final
	{
		std::vector<dx::XMFLOAT3> m_Vertices;
		std::vector<dx::XMFLOAT3> m_Normals;
		std::vector<dx::XMFLOAT2> m_Uvs;
		std::vector<UINT> m_Triangles;
	};

	class RenderBufferLoader final
	{
	public:
		typedef std::shared_ptr<RenderBuffers> ResourceType;

		enum class LoadMode : int
		{
			_3D,
			_2D,
			Undefined,
			ENUM_COUNT
		};

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
		void SetLoadMode(LoadMode const loadMode) NABI_NOEXCEPT;

	private:
		// 3D
		[[nodiscard]] std::shared_ptr<Mesh> Load3DMesh(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
		[[nodiscard]] dx::XMFLOAT2 ObjlVector2ToDxFloat2(objl::Vector2 const& vector2) const NABI_NOEXCEPT;
		[[nodiscard]] dx::XMFLOAT3 ObjlVector3ToDxFloat3(objl::Vector3 const& vector3) const NABI_NOEXCEPT;

		// 2D
		[[nodiscard]] std::shared_ptr<Sprite> Load2DSprite(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;

		LoadMode m_LoadMode = LoadMode::Undefined;
	};
} // namespace nabi::Rendering