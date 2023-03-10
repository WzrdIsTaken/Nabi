#pragma once
#include "Core.h"

#include "Containers\UVs.h"

namespace ecs::TextModule
{
	std::vector<nabi::Rendering::UVs> CalculateStringUvs(std::string const& string, int const asciiShift, dx::XMINT2 const textureAtlas);
	nabi::Rendering::UVs CalculateCharacterUvs(char const character, int const asciiShift, dx::XMINT2 const textureAtlas);
} // namespace ecs::TextModule
