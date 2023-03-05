#include "EngineCore.h"
#include "WinCore.h"

#include "stb_image.h"

#include "Sprite\Sprite.h"

namespace nabi::Rendering
{
	SpriteLoader::ResourceType SpriteLoader::operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT
	{
		// Load the image
		//int width, height, channels;
		//int constexpr desiredChannels = 4;
		//unsigned char const* const imageData = stbi_load(resourcePath.c_str(), &width, &height, &channels, desiredChannels);

		// Release the image
		//stbi_image_free(imageData);



		return ResourceType();
	}
} // namespace nabi::Rendering
