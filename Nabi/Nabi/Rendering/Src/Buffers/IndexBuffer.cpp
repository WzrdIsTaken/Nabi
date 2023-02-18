#include "EngineCore.h"
#include "WinCore.h" // Context includes RenderCommand, which includes WinCore - so things get messy if we don't include it here

#include "Buffers\IndexBuffer.h"

#include "Context.h"

namespace nabi::Rendering
{
	IndexBuffer nabi::Rendering::IndexBufferLoader::operator()(Mesh const& mesh, nabi::Context const& context)
	{
		return {};
	}
} // namespace nabi::Rendering