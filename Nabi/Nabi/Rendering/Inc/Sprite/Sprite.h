#pragma once
#include "EngineCore.h"

#include "Buffers\IndexBuffer.h"
#include "Buffers\VertexBuffer.h"

namespace nabi
{
	struct Context;
} // namespace nabi

namespace nabi::Rendering
{
	struct Sprite final
	{
		IndexBuffer m_IndexBuffer;
		VertexBuffer m_VertexBuffer;

		// TODO do this for now. but could have a "buffer component" which is part of the render pipleline. then literraly just have a mesh/tag sprite component
	};

	class SpriteLoader final
	{
	public:
		typedef std::shared_ptr<Sprite> ResourceType;

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
