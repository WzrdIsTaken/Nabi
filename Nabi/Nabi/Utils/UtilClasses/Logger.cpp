#include "Logger.h"

#include <iostream>

#include "../DebugUtils.h"

#ifdef _DEBUG

TSINGLETON_INSTANCE(nabi::Utils::DebugUtils::Logger)

namespace nabi::Utils::DebugUtils
{
	Logger::Logger()
		: m_LogLevel(c_LogLevelAll)
		, m_LogLevels{
			{ "INFO",        c_LogLevelInfo },
			{ "WARN",        c_LogLevelWarn },
			{ "ERROR",       c_LogLevelError },
			{ "FATAL ERROR", c_LogLevelFatal }
		}
		, m_LogMessageCount(0)
	{
	}

	void Logger::Log(std::string_view const severity, std::ostringstream const& logStream)
	{
		int const logLevel = m_LogLevels.at(severity);
		if (m_LogLevel <= logLevel)
		{
			switch (logLevel)
			{
				case c_LogLevelInfo:
				case c_LogLevelWarn:
					std::cout << logStream.str();
					break;
				case c_LogLevelError:
				case c_LogLevelFatal:
					std::cerr << logStream.str();
					break;
				default:
					ASSERT_FAIL("The log level is not accounted for!");
					break;
			}
		}

		m_LogMessageCount++;
	}

	void Logger::SetLogLevel(int const logLevel)
	{
		m_LogLevel = logLevel;
	}

	Logger::LogCount Logger::GetLogCount() const
	{
		return m_LogMessageCount;
	}
} // namespace nabi::Utils::DebugUtils
#endif // ifdef _DEBUG
