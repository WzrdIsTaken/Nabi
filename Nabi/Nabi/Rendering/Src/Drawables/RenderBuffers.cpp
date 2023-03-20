#include "EngineCore.h"
#include "WinCore.h"

#include "Drawables\RenderBuffers.h"

#include "OBJLoader.h"

#include "Buffers\IndexBuffer.h"
#include "Buffers\VertexBuffer.h"
#include "Context.h"
#include "CoreModules\RenderModule.h"
#include "DebugUtils.h"

namespace nabi::Rendering
{
	RenderBufferLoader::ResourceType RenderBufferLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		ResourceType result = nullptr;

		switch (m_LoadMode)
		{
			case LoadMode::_3D:
			{
				ResourceType primativeBuffers = CheckAndLoadPrimative(context, resourcePath);
				result = primativeBuffers != nullptr ? primativeBuffers : Load3DMesh(resourcePath, context);
				break;
			}		
			case LoadMode::_2D:
				result = Load2DSprite(resourcePath, context);
				break;
			case LoadMode::Undefined:
				LOG(LOG_PREP, LOG_ERROR, LOG_CATEGORY_RENDERING << "LoadMode is undefined! Did you forget to set it?" << ENDLINE);
				[[fallthrough]];
			default:
				ASSERT_FAIL("Unspecified LoadMode!");
				break;
		}

		return result;
	}

	void RenderBufferLoader::SetLoadMode(LoadMode const loadMode) NABI_NOEXCEPT
	{
		m_LoadMode = loadMode;
	}

	void RenderBufferLoader::SetSpriteSheetProperties(UVs const uvs)
	{
		m_SpriteUVs = uvs;
	}

#pragma region 3D

	std::shared_ptr<Mesh> RenderBufferLoader::Load3DMesh(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		ResourceType mesh = std::make_shared<Mesh>();
		MeshData meshData;

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
				dx::XMFLOAT2 const uv = dx::XMFLOAT2(vertex.TextureCoordinate.X, 1.0f - vertex.TextureCoordinate.Y);
				dx::XMFLOAT3 const normal = ObjlVector3ToDxFloat3(vertex.Normal);

				meshData.m_Vertices.push_back(vertice);
				meshData.m_Uvs.push_back(uv);
				meshData.m_Normals.push_back(normal);
			}
			for (size_t i = 0; i < loader.LoadedIndices.size(); i += 3)
			{
				meshData.m_Triangles.push_back(static_cast<UINT>(loader.LoadedIndices[i]));
				meshData.m_Triangles.push_back(static_cast<UINT>(loader.LoadedIndices[i + 2]));
				meshData.m_Triangles.push_back(static_cast<UINT>(loader.LoadedIndices[i + 1]));
			}

			// Log the successful load of the model
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_RENDERING << "Loaded a model with path " << WRAP(resourcePath, "'") << ". " <<
				"Model has " << meshData.m_Vertices.size() << " vertices and " << meshData.m_Triangles.size() << " triangles." << ENDLINE);
		}

		// Create an index and vertex buffer for the mesh (would have already returned the empty mesh by this point is the load failed)
		IndexBufferLoader const indexBufferLoader;
		mesh->m_IndexBuffer = indexBufferLoader(meshData, context);

		VertexBufferLoader const vertexBufferLoader;
		mesh->m_VertexBuffer = vertexBufferLoader(meshData, context);

		// Return the resource
		return mesh;
	}

	RenderBufferLoader::ResourceType RenderBufferLoader::CheckAndLoadPrimative(nabi::Context const& context, std::string const& resourcePath) const NABI_NOEXCEPT
	{
		// Extract the primatives dimensions
		// Format: PrimativeName=dim1xdim2xetc
		auto extractDimensionsFromPrimative =
			[](std::string const& primative) -> std::optional<std::vector<int>>
			{
				std::optional<std::vector<int>> primativeDimensionsReturn = {};
				std::vector<int> primativeDimensions = {};
			
				size_t const equals = primative.find_last_of("=") + 1u;
				if (equals != std::string::npos)
				{
					size_t lastDivider = equals;
					while (lastDivider != std::string::npos)
					{
						size_t const divider = primative.find_first_of("x", lastDivider);
						std::string dimensionAsStr;

						if (divider != std::string::npos)
						{
							dimensionAsStr = primative.substr(equals, divider - equals);
							lastDivider = divider + 1u;
						}
						else
						{
							dimensionAsStr = primative.substr(lastDivider);
							lastDivider = std::string::npos;
						}

						int const dimension = std::stoi(dimensionAsStr);
						primativeDimensions.push_back(dimension);
						
					}
				}

				if (primativeDimensions.size() != 0u)
				{
					primativeDimensionsReturn = primativeDimensions;
				}

				return primativeDimensionsReturn;
			};

		entt::hashed_string constexpr primativeSphere = "PrimativeSphere";
		entt::hashed_string constexpr primativeCube = "PrimativeCube";
		std::array<std::string_view, 2> constexpr primatives = { primativeSphere.data(), primativeCube.data() };

		ResourceType buffers = nullptr;

		for (std::string_view const primative : primatives)
		{
			if (resourcePath.find(primative) != std::string::npos)
			{
				entt::hashed_string const primativeNameHash = entt::hashed_string(primative.data());
				std::optional<std::vector<int>> const primativeDimensions = extractDimensionsFromPrimative(resourcePath);

				if (primativeDimensions.has_value())
				{
					std::vector<int> const& dimensions = primativeDimensions.value();

					switch (primativeNameHash)
					{
						case primativeSphere:
						{
							RenderBuffers const sphereBuffers = ecs::RenderModule::CreatePrimativeSphere(context, dimensions.at(0), dimensions.at(1));
							buffers = std::make_shared<Mesh>(sphereBuffers);
							break;
						}
						case primativeCube:
						{
							RenderBuffers const cubeBuffers = ecs::RenderModule::CreatePrimativeCube(
								context, 
								static_cast<float>(dimensions.at(0)),
								static_cast<float>(dimensions.at(1)),
								static_cast<float>(dimensions.at(2))
							);
							buffers = std::make_shared<Mesh>(cubeBuffers);
							break;
						}
						default:
							ASSERT_FAIL("Undefined primative name!");
							break;
					}
				}

				break;
			}
		}

		return buffers;
	}

	dx::XMFLOAT2 RenderBufferLoader::ObjlVector2ToDxFloat2(objl::Vector2 const vector2) const NABI_NOEXCEPT
	{
		dx::XMFLOAT2 const float2 = dx::XMFLOAT2(vector2.X, vector2.Y);
		return float2;
	}

	dx::XMFLOAT3 RenderBufferLoader::ObjlVector3ToDxFloat3(objl::Vector3 const vector3) const NABI_NOEXCEPT
	{
		dx::XMFLOAT3 const float3 = dx::XMFLOAT3(vector3.X, vector3.Y, vector3.Z);
		return float3;
	}

#pragma endregion

#pragma region 2D

	std::shared_ptr<Sprite> RenderBufferLoader::Load2DSprite(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		ResourceType sprite = std::make_shared<Sprite>();
		SpriteData spriteData;

		// For 2D, we don't need any special loading - we can just predefine the values

		// Index
		std::vector<UINT> const/*expr ):*/ indices2D =
		{
			// First triangle
			0u, 1u, 2u,
			// Second triangle
			0u, 2u, 3u
		};
		spriteData.m_Triangles = indices2D;

		// Vertex
		std::vector<dx::XMFLOAT3> const/*expr :(*/ vertices2D =
		{
			// Bottom left
			{ -1.0f, -1.0f, 0.0f },
			// Top Left
			{ -1.0f, 1.0f, 0.0f },
			// Top right
			{ 1.0f, 1.0f, 0.0f  },
			// Bottom left
			{ 1.0f, -1.0f, 0.0f }
		};
		spriteData.m_Vertices = vertices2D;

		// Uvs
		std::vector<dx::XMFLOAT2> const uvs2D = UV::CreateSpriteUVs(m_SpriteUVs);
		spriteData.m_Uvs = uvs2D;

		// Currently in Nabi normals aren't used for 2d sprites

		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_RENDERING << "Loaded a sprite with path " << WRAP(resourcePath, "'") << ENDLINE);

		// Create the buffers
		IndexBufferLoader const indexBufferLoader;
		sprite->m_IndexBuffer = indexBufferLoader(spriteData, context);

		VertexBufferLoader const vertexBufferLoader;
		sprite->m_VertexBuffer = vertexBufferLoader(spriteData, context);

		return sprite;
	}

#pragma endregion
} // namespace nabi::Rendering
