#pragma once
#include "Core.h"

#include "Containers\UVs.h"

namespace ecs
{
	struct TextComponent;
	struct TransformComponent;
} // namespace ecs

namespace ecs::TextModule
{
	void UpdateTextContent(nabi::Context& context, entt::entity const textEntity, std::string const& newContent);
	void UpdateTextContent(nabi::Context& context, TextComponent& textComponent, std::string const& newContent);

	void CenterText(nabi::Context& context, entt::entity const textEntity, dx::XMFLOAT3 const* const position = nullptr);
	void CenterText(nabi::Context& context, TextComponent const& textComponent, TransformComponent& textTransformComponent, dx::XMFLOAT3 const* const position = nullptr);

	std::vector<nabi::Rendering::UVs> CalculateStringUvs(std::string const& string, int const asciiShift, dx::XMINT2 const textureAtlas);
	nabi::Rendering::UVs CalculateCharacterUvs(char const character, int const asciiShift, dx::XMINT2 const textureAtlas);
} // namespace ecs::TextModule
