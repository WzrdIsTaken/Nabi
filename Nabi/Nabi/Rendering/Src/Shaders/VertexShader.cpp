#include "EngineCore.h"
#include "WinCore.h"

#include "Shaders\PixelShader.h"

#include "Context.h"

namespace nabi::Rendering
{
	VertexShaderLoader::ResourceType VertexShaderLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		// Nabi will only ever use 3d meshs, so this is an ok bit of jank
		VertexShader const vertexShader = context.m_RenderCommand->CreateVertexShader(resourcePath, c_MeshInputLayout);
		return std::make_unique<VertexShader>(vertexShader);
	}
} // namespace nabi::Rendering
