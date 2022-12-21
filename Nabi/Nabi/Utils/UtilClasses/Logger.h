#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "../../Templates/Singleton.h"

#ifdef _DEBUG

namespace nabi::Utils::DebugUtils
{
	// Some new defines to be used with SetLogLevel. Macros so they match up stylistically with the logging levels defined in DebugUtils
#define LOG_LEVEL_ALL  "this string could be anything!"
#define LOG_LEVEL_NONE "this one as well, bananas!"

	/// <summary>
	/// Handles all* logging in the game. Pretty important!
	/// *almost...
	///
	/// </summary>
	class Logger final : public nabi::Templates::TSingleton<Logger>
	{
	public:
		typedef unsigned long long LogCount;

		Logger();

		/// <summary>
		/// Logs a message to the console. Info and warn use cout, error and fatal use cerr
		/// </summary>
		/// <param name="severity">- The level of the error</param>
		/// <param name="logStream">- The message to log</param>
		void Log(std::string_view const severity, std::ostringstream const& logStream);

		/// <summary>
		/// Sets m_LogLevel. Only debug messages with a level equal or above to m_LogLevel will be displayed
		/// </summary>
		/// <param name="logLevel">- The new log level</param>
		void SetLogLevel(std::string_view const logLevel);

		/// <summary>
		/// Gets the number of messages that have been logged this session
		/// </summary>
		/// <returns>The number of messages logged this session (as an unsigned long long)</returns>
		LogCount GetLogCount() const;

	private:
		std::unordered_map<std::string_view, int> const m_LogLevels;
		int m_LogLevel;

		// This guy is kinda just for fun
		LogCount m_LogMessageCount;
	};
} // namespace nabi::Utils::DebugUtils

#endif // ifdef _DEBUG
