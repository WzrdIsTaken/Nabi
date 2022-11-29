#pragma once
#include <iostream>

#ifndef _DEBUG
	#define ASSERT_FATAL(condition, message) ((void)0)
	#define ASSERT(condition, message) ((void)0) 

	#define ASSERT_FAIL_FATAL(message) ASSERT_FATAL(false, message)
	#define ASSERT_FAIL(message) ASSERT(false, message)

	#define LOG(message) ((void)0)
#else
#define ASSERT_FATAL(condition, message) \
		if (!(condition)) \
		{ \
			std::cout << "TODO: ASSERT FATAL: " << message << std::endl; \
		}
#define ASSERT(condition, message) \
		if (!(condition)) \
		{ \
			std::cout << "TODO: ASSERT: " << message << std::endl; \
		}

#define ASSERT_FAIL_FATAL(message) ASSERT_FATAL(false, message)
#define ASSERT_FAIL(message) ASSERT(false, message)

#define LOG(prep, severity, message) \
		if (std::strcmp(severity, LOG_ERROR) == 0) \
		{ \
			std::cerr << prep << severity << message; \
		} \
		else \
		{ \
			std::cout << prep << severity << message; \
		}
#endif // ifndef _DEBUG

#define TOSTRING(thing) #thing
#define CONCAT(A, B) A ## B

#define FULL_LOCATION  __FILE__ << " " << __LINE__ << " "
#define SHORT_LOCATION \
	std::string(__FILE__).substr(std::string(__FILE__).find_last_of('\\') + 1, std::string(__FILE__).size()) << " " << __LINE__ << " "

#define LOG_PREP SHORT_LOCATION // Can add anything else on here if need be
#define LOG_INFO    "INFO: "
#define LOG_WARNING "WARNING: "
#define LOG_ERROR   "ERROR: "

#define ENDLINE std::endl
#define WRAP(item, wrapCharacter) wrapCharacter << item << wrapCharacter
#define SPACE(numberOfSpaces) std::string(numberOfSpaces, ' ')
