#pragma once

namespace nabi::Reflection
{
	/// <summary>
	/// We need to reflect our base types
	/// </summary>
	class BaseTypeReflection
	{
		friend class BaseTypeReflector;

		class BaseTypeReflector
		{
		public:
			/// <summary>
			/// A static constructor to reflect all base types
			/// </summary>
			BaseTypeReflector();
		};

		static BaseTypeReflector baseTypeReflectorInstance;
	};
} // namespace nabi::Reflection
