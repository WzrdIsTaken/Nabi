#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "../../Templates/Singleton.h"

// Todo - Would be good to make these log levels an enum and be able to convert enum->string. Atm its a little jank
// ^ Or just have the log levels take in the same string as debug utils?

#ifdef _DEBUG

namespace nabi::Utils::DebugUtils
{
	/// <summary>
	/// Handles all logging in the game. Atm its a little rough around the edges, but servicable. Hopefully that will change soon!
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
		void SetLogLevel(int const logLevel);

		/// <summary>
		/// Gets the number of messages that have been logged this session
		/// </summary>
		/// <returns>The number of messages logged this session (as an unsigned long long int)</returns>
		LogCount GetLogCount() const;

		// Log Levels
		static int constexpr c_LogLevelAll   = -1;
		static int constexpr c_LogLevelNone  = 9999;

		static int constexpr c_LogLevelInfo  = 0;
		static int constexpr c_LogLevelWarn  = 1;
		static int constexpr c_LogLevelError = 2;
		static int constexpr c_LogLevelFatal = 3;

	private:
		std::unordered_map<std::string_view, int> const m_LogLevels;
		int m_LogLevel;

		// This guy is kinda just for fun
		LogCount m_LogMessageCount;
	};
} // namespace nabi::Utils::DebugUtils

#endif // ifdef _DEBUG
