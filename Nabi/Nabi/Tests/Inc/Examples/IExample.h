#pragma once
#include "TestCore.h"

// Examples are kinda like me testing stuff out, rather than an automated unit test ran on project startup

namespace nabitest::Examples
{
	class IExample abstract
	{
	public:
		virtual bool Init(nabi::Context& context)
		{
			m_Context = std::move(context);
			return true;
		}
		virtual bool Update() { return false; }
		virtual bool Render() { return false; }

	private:
		nabi::Context& m_Context; // basically an interface
	};
} // namespace nabitest::Examples
