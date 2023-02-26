#include "EngineCore.h"
#include "WinCore.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\IndexBuffer.h"

#include "Context.h"
#include "Model\Mesh.h"

namespace nabi::Rendering
{
	IndexBuffer nabi::Rendering::IndexBufferLoader::operator()(Mesh const& mesh, nabi::Context const& context) const NABI_NOEXCEPT
	{
		IndexBuffer const indexBuffer = context.m_RenderCommand->CreateIndexBuffer(mesh.m_Triangles);
		return indexBuffer;
	}
} // namespace nabi::Rendering
