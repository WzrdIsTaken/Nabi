#pragma once

#ifndef _DEBUG
	#define MAKE_TESTABLE
#else
	#define MAKE_TESTABLE virtual 

	namespace nabi::Utils::TestUtils
	{
		constexpr int c_TestResultSuccess = 0;
		constexpr int c_TestResultFail = 1;
	} // nabi nabi::Utils::TestUtils
#endif // #ifdef _DEBUG
