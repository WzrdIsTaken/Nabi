#include "EngineCore.h"
#include "WinCore.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\VertexBuffer.h"

#include "Context.h"

namespace nabi::Rendering
{
	VertexBuffer nabi::Rendering::VertexBufferLoader::operator()(Mesh const& mesh, nabi::Context const& context)
	{
		return {};
	}
} // namespace nabi::Rendering