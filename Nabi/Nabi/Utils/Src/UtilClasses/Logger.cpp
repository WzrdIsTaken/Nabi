#include "../../../Core/EngineCore.h"

#include "../../Inc/UtilClasses/Logger.h"

#include <iostream>

#include "../../../Libraries/Inc/entt/entt.h"
#include "../../Inc/DebugUtils.h"

#ifdef USE_DEBUG_UTILS

TSINGLETON_INSTANCE(nabi::Utils::DebugUtils::Logger)

namespace nabi::Utils::DebugUtils
{
	Logger::Logger() NABI_NOEXCEPT
		: m_LogLevels{
			{ LOG_LEVEL_ALL,  -1},
			{ LOG_INFO,       0 },
			{ LOG_WARNING,    1 },
			{ LOG_ERROR,      2 },
			{ LOG_FATAL,      3 },
			{ LOG_LEVEL_NONE, 99}
		}
		, m_LogLevel(m_LogLevels.at(LOG_LEVEL_ALL))
		, m_LogMessageCount(0)
	{
	}

	void Logger::Log(std::string_view const severity, std::ostringstream const& logStream) NABI_NOEXCEPT
	{
		int const logLevel = m_LogLevels.at(severity);
		if (m_LogLevel <= logLevel)
		{
			entt::hashed_string const severityHash = entt::hashed_string(severity.data());
			switch (severityHash)
			{
			case entt::hashed_string(LOG_INFO):
				[[fallthrough]];
			case entt::hashed_string(LOG_WARNING):
				std::cout << logStream.str();
				break;
			case entt::hashed_string(LOG_ERROR):
				[[fallthrough]];
			case entt::hashed_string(LOG_FATAL):
				std::cerr << logStream.str();
				break;
			default:
				ASSERT_FAIL("The log level is not accounted for!");
				break;
			}
		}

		m_LogMessageCount++;
	}

	void Logger::LogRaw(std::ostringstream const& logStream) NABI_NOEXCEPT
	{
		std::string const warningMessage = "[Logger::LogRaw] WARNING - LOGGER NOT INITIALIZED |";
		std::cout << warningMessage << " " << logStream.str();
	}

	void Logger::SetLogLevel(std::string_view const logLevel) NABI_NOEXCEPT
	{
		ASSERT_FATAL(m_LogLevels.find(logLevel) != m_LogLevels.end(), "The log level is not defined!");
		m_LogLevel = m_LogLevels.at(logLevel);
	}

	Logger::LogCount Logger::GetLogCount() const NABI_NOEXCEPT
	{
		return m_LogMessageCount;
	}
} // namespace nabi::Utils::DebugUtils

#endif // ifdef USE_DEBUG_UTILS
