#include "EngineCore.h"
#include "WinCore.h"

#include "Model\Mesh.h"

#include "OBJLoader.h"

#include "Context.h"

namespace nabi::Rendering
{
	MeshLoader::ResourceType MeshLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		return std::make_unique<Mesh>();
	}
} // namespace nabi::Rendering
