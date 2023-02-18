#pragma once
#include "TestCore.h"

#include "IExample.h"

namespace nabitest::Examples
{
	class BasicTriangle : public IExample
	{
	public:
		bool Init(nabi::Context& context) override;
		bool Update() override;
		bool Render() override;

	private:
		nabi::Context m_Context;
	};
} // namespace nabitest::Examples
