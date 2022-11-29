#pragma once

// Contains a bunch of core test includes to make life easier

// Nabi Core
#include "../Core.h"

// Mock Objects
#include "MockObjects/MockComponent.h"
#include "MockObjects/MockCustomDataType.h"
#include "MockObjects/MockGameCore.h"
#include "MockObjects/MockSystem.h"

// Basic comparison struct. Might move this in the future...
#ifdef _DEBUG
namespace nabitest
{
	template<typename T>
	struct Comparison
	{
		Comparison()
			: m_Expected{}
			, m_Actual{}
		{
		}

		explicit Comparison(T expected)
			: m_Expected(expected)
			, m_Actual{}
		{
		}

		Comparison(T expected, T actual)
			: m_Expected(expected)
			, m_Actual(actual)
		{
		}

		T m_Expected;
		T m_Actual;
	};
}
#endif // #ifdef _DEBUG
