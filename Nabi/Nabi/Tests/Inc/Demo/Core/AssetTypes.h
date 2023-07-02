#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

namespace core
{	 
	enum class AssetType : uint64_t
	{
		Model  = 1 << 1,
		Sprite = 1 << 2,
		Text   = 1 << 3,
		Audio  = 1 << 4,

		All = ~0
	};
	DEFINE_ENUM_FLAG_OPERATORS(AssetType)

	namespace AssetTypes
	{
		auto constexpr c_RenderableAssets = AssetType::Model | AssetType::Sprite | AssetType::Text;
		auto constexpr c_AudioAssets      = AssetType::Audio;
	} // namespace AssetTypes
} // namespace core

#endif //ifdef INCLUDE_DEMO
