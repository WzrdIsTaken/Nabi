#pragma once

#include <algorithm>
#include <string>
#include <sstream>

// Forward Declarations
namespace entt
{
	class meta_any;
	struct meta_data;

	template<typename Char>
	class basic_hashed_string;
	using hashed_string = basic_hashed_string<char>;
}

// Handles the conversion of strings to types, and includes the basic type FromString methods

namespace nabi::Reflection::StringConverter
{
	/// <summary>
	/// Converts a string property value to its correct type, and assigns it to the meta property on the passed in meta object.
	/// </summary>
	/// <param name="propertyValue">- The string property value (eg: "1", "true")</param>
	/// <param name="propertyTypeHash">- The type of the property (eg: "bool" <- but a hashed representation)</param>
	/// <param name="metaObject">- The meta object to assign the converted property to</param>
	/// <param name="metaMember">- The meta property on the meta object to set to the  converted property's value</param>
	void ConvertFromString(std::string const& propertyValue, entt::hashed_string const& propertyTypeHash,
						   entt::meta_any& metaObject, entt::meta_data const& metaMember);

	/// <summary>
	/// Extracts the type from a namespace + type. Eg: MyNamespace::MyType -> MyType.
	/// Can also be used for extracting the values of enums. Eg: MyEnum::One -> One.
	/// Or just about anything else which meets this :: criteria I guess!
	/// </summary>
	/// <param name="typeInfoName">- The full type name</param>
	std::string ExtractTypeName(std::string_view const typeInfoName);
	void ExtractTypeName(std::string& typeInfoName);

	/// <summary>
	/// This specialised template functions convert a string to a type. They are used to support basic type reflection.
	/// </summary>
	/// <typeparam name="T">- The basic type</typeparam>
	/// <param name="string">- The string to convert to T</param>
	/// <returns>The converted type</returns>
	template<typename T>
	static T FromString(std::string const& string) {};

	// char
	template<>
	char FromString<char>(std::string const& string)
	{
		char const result = string.front();
		return result;
	}

	// int
	template<>
	int FromString<int>(std::string const& string)
	{
		int const result = std::stoi(string);
		return result;
	}

	// float
	template<>
	float FromString<float>(std::string const& string)
	{
		float const result = std::stof(string);
		return result;
	}

	// double
	template<>
	double FromString<double>(std::string const& string)
	{
		double const result = std::stod(string);
		return result;
	}

	// bool
	template<>
	bool FromString<bool>(std::string const& string)
	{
		std::string inputString = string; // unfortunate
		std::transform(inputString.begin(), inputString.end(), inputString.begin(), std::tolower);
		std::stringstream ss(string);

		bool result;
		ss >> std::boolalpha >> result;
		return result;
	}

	// string
	template<>
	std::string FromString<std::string>(std::string const& string)
	{
		std::string const result = string;
		return result;
	}
} // namespace nabi::Reflection::StringConverter
