#pragma once

#include <memory>

#include "../Utils/DebugUtils.h"

namespace nabi //::Templates
{
	template<class T>
	class TSingleton
	{
	public:
		static void CreateInstance()
		{
			ASSERT(!s_Instance, "Singleton already has an instance!")
			s_Instance = std::make_unique<T>();
		}

		static bool IsInstanceValid()
		{
			return s_Instance != nullptr;
		}

		static T* Instance()
		{
			ASSERT(!s_Instance, "Trying to access the singleton instance when it hasn't been created yet!");
			return s_Instance.get();
		}

	private:
		/*
		friend class SingletonCreator;

		class SingletonCreator
		{
		public:
			/// <summary>
			///
			/// </summary>
			SingletonCreator();
		};

		static SingletonCreator m_SingletonCreatorInstance;
		*/
		
		static std::unique_ptr<T> s_Instance;
	};

#define TSINGLETON_INSTANCE(_class) std::unique_ptr<_class> _class::s_Instance = nullptr;
} // namespace nabi //::Templates
