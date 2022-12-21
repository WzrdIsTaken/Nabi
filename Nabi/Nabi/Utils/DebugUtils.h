#pragma once

#include <sstream>

#include "UtilClasses/Logger.h"

#ifndef _DEBUG
	#define ASSERT_FATAL(condition, message) NOT_DEFINED
	#define ASSERT(condition, message) NOT_DEFINED 

	#define ASSERT_FAIL_FATAL(message) NOT_DEFINED
	#define ASSERT_FAIL(message) NOT_DEFINED

	#define LOG(message) NOT_DEFINED
	#define LOG_RAW(message) NOT_DEFINED

	#define FUNCTION_NOT_IMPLEMENTED NOT_DEFINED
#else
#define ASSERT_FATAL(condition, message) \
		if (!(condition)) \
		{ \
			LOG(LOG_PREP, LOG_FATAL, "TODO: ASSERT FATAL: " << message << ENDLINE); \
		}
#define ASSERT(condition, message) \
		if (!(condition)) \
		{ \
			LOG(LOG_PREP, LOG_ERROR, "TODO: ASSERT: " << message << ENDLINE); \
		}

#define ASSERT_FAIL_FATAL(message) ASSERT_FATAL(false, message)
#define ASSERT_FAIL(message) ASSERT(false, message)

#define LOG(prep, severity, message) \
		{ \
			std::ostringstream debugStream; \
			debugStream << prep << severity << LEVEL_MESSAGE_DIVIDER << message; \
			nabi::Utils::DebugUtils::Logger::Instance()->Log(severity, debugStream); \
		}
#define LOG_RAW(message) std::cout << message << std::endl;

#define FUNCTION_NOT_IMPLEMENTED ASSERT_FAIL("The function " << __FUNCTION__  << " is not implemented!");
#endif // ifndef _DEBUG

#define NOT_DEFINED ((void)0)

#define TOSTRING(thing) #thing
#define CONCAT(A, B) A ## B

#define FULL_LOCATION  __FILE__ << " " << __LINE__ << " "
#define SHORT_LOCATION \
	std::string(__FILE__).substr(std::string(__FILE__).find_last_of('\\') + 1, std::string(__FILE__).size()) << " " << __LINE__ <<

#define SHORT_LOCATION_END(line) SPACE(4 - std::string(TOSTRING(line)).length()) // Because of the way these macros work, we have to do this pass __LINE__ in TOSTRING stuff
#define LEVEL_MESSAGE_DIVIDER ": "

#define LOG_PREP SHORT_LOCATION SHORT_LOCATION_END(__LINE__) // Can add anything else on here if need be
#define LOG_INFO    "INFO"
#define LOG_WARNING "WARNING"
#define LOG_ERROR   "ERROR"
#define LOG_FATAL   "FATAL ERROR"

#define NEWLINE "\n"
#define ENDLINE std::endl
#define WRAP(item, wrapCharacter) wrapCharacter << item << wrapCharacter
#define SPACE(numberOfSpaces) std::string(numberOfSpaces, ' ')

#define INDENT_1 3
#define INDENT_2 6
#define INDENT_3 9
