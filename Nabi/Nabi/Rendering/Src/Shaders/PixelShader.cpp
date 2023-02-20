#include "EngineCore.h"
#include "WinCore.h"

#include "Shaders\PixelShader.h"

#include "Context.h"

namespace nabi::Rendering
{
	PixelShaderLoader::ResourceType PixelShaderLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		PixelShader const pixelShader = context.m_RenderCommand->CreatePixelShader(resourcePath);
		return std::make_shared<PixelShader>(pixelShader);
	}
} // namespace nabi::Rendering
