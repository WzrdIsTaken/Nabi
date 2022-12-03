#include "Logger.h"

#include <iostream>

//#include "../DebugUtils.h"

#ifdef _DEBUG
namespace nabi::Utils::DebugUtils
{
	//TSINGLETON_INSTANCE(Logger)

	//std::unique_ptr<Logger> Logger::m_Instance = nullptr; ???

	Logger::Logger()
		: m_LogLevel(0)
	{
		m_LogLevels = std::unordered_map<std::string_view, int> // todo should this be string view?
		{
			{ "INFO",  c_LogLevelInfo },
			{ "WARN",  c_LogLevelWarn },
			{ "ERROR", c_LogLevelError }
		};
	}

	void Logger::Log(std::string_view prep, std::string_view severity, std::string_view message)
	{
		int const logLevel = m_LogLevels[severity];
		if (logLevel > m_LogLevel)
		{
			return;
		}

		std::string const logMessage = std::string(prep) + std::string(severity) + std::string(message); // todo can this be improved?
		switch (logLevel)
		{
		case c_LogLevelInfo:
		case c_LogLevelWarn:
			std::cout << logMessage;
			break;
		case c_LogLevelError:
			std::cerr << logMessage;
			break;
		}

		// todo - assert undefined log level (default case)
	}
} // namespace nabi::Utils::DebugUtils
#endif // ifdef _DEBUG
