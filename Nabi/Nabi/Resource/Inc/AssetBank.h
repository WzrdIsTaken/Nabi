#pragma once
#include "EngineCore.h"

namespace nabi
{
	struct Context;
}

namespace nabi::Resource
{
	class AssetBank abstract
	{
	public:
		AssetBank(nabi::Context& context)
			: m_Context(context)
		{
		}
		virtual ~AssetBank() {}

		virtual bool LoadAssets()   = 0;
		virtual bool UnloadAssets() = 0;

	protected:
		nabi::Context& m_Context;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(AssetBank)
	};
} // namespace nabi::Resource
