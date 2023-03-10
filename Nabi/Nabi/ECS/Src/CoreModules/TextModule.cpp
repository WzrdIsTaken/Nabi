#include "Core.h"

#include "CoreModules\TextModule.h"

namespace ecs::TextModule
{
	std::vector<nabi::Rendering::UVs> CalculateStringUvs(std::string const& string, int const asciiShift, dx::XMINT2 const textureAtlas)
	{
		std::vector<nabi::Rendering::UVs> stringUvs = {};
		stringUvs.reserve(string.length());

		for (char const character : string)
		{
			nabi::Rendering::UVs const characterUv = CalculateCharacterUvs(character, asciiShift, textureAtlas);
			stringUvs.emplace_back(characterUv);
		}

		return stringUvs;
	}

	nabi::Rendering::UVs CalculateCharacterUvs(char const character, int const asciiShift, dx::XMINT2 const textureAtlas)
	{
		nabi::Rendering::UVs characterUvs = {};
		dx::XMFLOAT2 u, v;

		// We can think of the texture atlas as like a grid the sprites are on
		float const textureAtlasX = static_cast<float>(textureAtlas.x);
		float const textureAtlasY = static_cast<float>(textureAtlas.y);

		// Ok so this is the magic bit. The code is calculating the UV coordinates a character in the texture atlas. 
		// By subtracting 'asciiShift' from the char, we get the index of the character in the atlas
		// The reason for 1.0f/~ is to normalize the u between 0 and 1
		// The reason for the 'index %' is to get the result of textureAtlas.x / textureAtlas.x within the bounds of the sprite sheet
		int const index = static_cast<int>(character) - asciiShift;
		u = { 1.0f / textureAtlasX, 1.0f / textureAtlasY };
		v = { index % textureAtlas.x / textureAtlasX, (index / textureAtlas.x) / textureAtlasY };
		u.x += v.x;
		u.y += v.y;

		characterUvs.m_U1 = u.x; characterUvs.m_U2 = u.y;
		characterUvs.m_V1 = v.x; characterUvs.m_V2 = v.y;
		return characterUvs;
	}
} // namespace ecs::TextModule
