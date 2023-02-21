#include "EngineCore.h"
#include "WinCore.h"

#include "Model\Mesh.h"

#include "OBJLoader.h"

#include "Buffers\IndexBuffer.h"
#include "Buffers\VertexBuffer.h"
#include "Context.h"
#include "DebugUtils.h"

namespace nabi::Rendering
{
	MeshLoader::ResourceType MeshLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		ResourceType mesh = std::make_shared<Mesh>();

		// Load the mesh
		{
			// Just using obj files to get something working quickly. A todo might be to convert to glTF in the future
			objl::Loader loader;
			bool const loaded = loader.LoadFile(resourcePath);

			if (!loaded)
			{
				ASSERT_FAIL("Model at path " << WRAP(resourcePath, "'") << " failed to load!");
				return mesh;
			}

			for (objl::Vertex const& vertex : loader.LoadedVertices)
			{
				dx::XMFLOAT3 const vertice = ObjlVector3ToDxFloat3(vertex.Position);
				dx::XMFLOAT2 const uv      = dx::XMFLOAT2(vertex.TextureCoordinate.X, 1.0f - vertex.TextureCoordinate.Y);
				dx::XMFLOAT3 const normal  = ObjlVector3ToDxFloat3(vertex.Normal);

				mesh->m_Vertices.push_back(vertice);
				mesh->m_Uvs.push_back(uv);
				mesh->m_Normals.push_back(normal);
			}
			for (size_t i = 0; i < loader.LoadedIndices.size(); i += 3)
			{
				mesh->m_Triangles.push_back(static_cast<UINT>(loader.LoadedIndices[i]));
				mesh->m_Triangles.push_back(static_cast<UINT>(loader.LoadedIndices[i + 2]));
				mesh->m_Triangles.push_back(static_cast<UINT>(loader.LoadedIndices[i + 1]));
			}

			// Log the successful load of the model
			LOG(LOG_PREP, LOG_INFO, "Loaded a model with path " << WRAP(resourcePath, "'") << ". " <<
				"Model has " << mesh->m_Vertices.size() << " vertices and " << mesh->m_Triangles.size() << " triangles." << ENDLINE);
		}

		// Create an index and vertex buffer for the mesh (would have already returned the empty mesh by this point is the load failed)
		IndexBufferLoader const indexBufferLoader;
		mesh->m_IndexBuffer = indexBufferLoader(*mesh, context);

		VertexBufferLoader const vertexBufferLoader;
		mesh->m_VertexBuffer = vertexBufferLoader(*mesh, context);

		// Return the resource
		return mesh;
	}

	dx::XMFLOAT2 MeshLoader::ObjlVector2ToDxFloat2(objl::Vector2 const& vector2) const NABI_NOEXCEPT
	{
		dx::XMFLOAT2 const float2 = dx::XMFLOAT2(vector2.X, vector2.Y);
		return float2;
	}

	dx::XMFLOAT3 MeshLoader::ObjlVector3ToDxFloat3(objl::Vector3 const& vector3) const NABI_NOEXCEPT
	{
		dx::XMFLOAT3 const float3 = dx::XMFLOAT3(vector3.X, vector3.Y, vector3.Z);
		return float3;
	}
} // namespace nabi::Rendering
