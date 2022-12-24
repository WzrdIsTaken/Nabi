#pragma once

#include <string>

#include "../../Libraries/entt/entt.h"
#include "../MetaECSTypes.h"

namespace nabi::Reflection
{
	/// <summary>
	/// Used to override certain properties on a runtime created entity
	/// </summary>
	class EntityPropertyList final
	{
	public:
		/// <summary>
		/// Adds a property that will be overriden on whatever entity is created using this list
		/// </summary>
		/// <param name="component">- The component that the property to override is on</param>
		/// <param name="propertyName">- The name of the property which will be overriden</param>
		/// <param name="propertyValue">- The overriden properties new value</param>
		void AddProperty(std::string_view const component, std::string_view const propertyName, std::string_view const propertyValue);
		/// <summary>
		/// Changes a properties value of an item in the overriden properties list
		/// </summary>
		/// <param name="component">- The component that the property to override is on</param>
		/// <param name="propertyName">- The name of the property which will be overriden</param>
		/// <param name="propertyValue">- The overriden properties updated value</param>
		void UpdateProperty(std::string_view const component, std::string_view const propertyName, std::string_view const propertyValue);
		/// <summary>
		/// Removes a property from the overriden properties list
		/// </summary>
		/// <param name="component">- The component that the property to remove is on</param>
		/// <param name="propertyName">- The name of the property to remove</param>
		void RemoveProperty(std::string_view const component, std::string_view const propertyName);

	private:
		struct PropertyComponentPair final
		{
			entt::hashed_string m_ComponentHash;
			PropertyData m_PropertyOverride;

			bool operator == (PropertyComponentPair const rhs) const
			{
				return this->m_ComponentHash == rhs.m_ComponentHash &&
					   this->m_PropertyOverride.m_Id == rhs.m_PropertyOverride.m_Id;
			}
		};

		/// <summary>
		/// Finds a property in m_OverriddenProperties 
		/// </summary>
		/// <param name="component">- The name of the component that the property to find is on</param>
		/// <param name="propertyName">- The name of the property to find</param>
		/// <returns></returns>
		PropertyComponentPair& FindPropertyComponentPair(std::string_view const component, std::string_view const propertyName);

		std::vector<PropertyComponentPair> m_OverriddenProperties;
	};
} // namespace nabi::Reflection
