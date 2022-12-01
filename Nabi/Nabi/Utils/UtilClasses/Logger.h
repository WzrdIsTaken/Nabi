#pragma once

#include <string>
#include <unordered_map>

// NOTE - THIS IS VERY WIP, BUT WILL DEFO BE IN HERE ONE DAY

#ifdef _DEBUG
namespace nabi::Utils::DebugUtils
{
	class Logger // Todo: Make inherit from Singleton
	{
	public:
		Logger();

		void Log(std::string_view prep, std::string_view severity, std::string_view message);

		// Todo - log to file function

	private:
		std::unordered_map<std::string_view, int> m_LogLevels; // todo - make const?

		static int constexpr c_LogLevelInfo  = 0; // todo - put in init list? todo 2 - can be like rtti enum? <-- really should. then wouldnt need this map or anything
		static int constexpr c_LogLevelWarn  = 1; // else just use entt::hashed_string
		static int constexpr c_LogLevelError = 2;

		int m_LogLevel;
	};
} // namespace nabi::Utils::DebugUtils
#endif // ifdef _DEBUG
