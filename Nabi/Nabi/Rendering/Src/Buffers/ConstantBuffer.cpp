#include "EngineCore.h"
#include "WinCore.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\ConstantBuffer.h"

#include "Context.h"

namespace nabi::Rendering
{
	ConstantBuffer nabi::Rendering::ConstantBufferLoader::operator()(UINT const byteWidth, nabi::Context const& context) const NABI_NOEXCEPT
	{
		ConstantBuffer const constantBuffer = context.m_RenderCommand->CreateConstantBuffer(byteWidth);
		return constantBuffer;
	}
} // namespace nabi::Rendering
