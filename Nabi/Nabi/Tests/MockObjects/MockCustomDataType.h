#pragma once

#include "../../Core.h"

#include "../../Reflection/StringConverter.h"
#include "../../Utils/StringUtils.h"

#ifdef _DEBUG

namespace nabitest
{
	struct MockCustomDataType
	{
		double m_DoubleType;
		bool m_BoolType;

		static MockCustomDataType FromString(std::string const& string)
		{
			char const delimiter = ',';
			size_t const expectedValues = 2;
			std::vector<std::string_view> const splitString = nabi::Utils::StringUtils::SplitString(string, delimiter, expectedValues);

			double const doubleType = nabi::Reflection::StringConverter::FromString<double>(splitString[0].data());
			bool const boolType = nabi::Reflection::StringConverter::FromString<bool>(splitString[1].data());

			MockCustomDataType const result = { doubleType, boolType };
			return result;
		}

		bool operator == (MockCustomDataType const rhs) const
		{
			return this->m_DoubleType == rhs.m_DoubleType && 
				   this->m_BoolType   == rhs.m_BoolType;
		}
	};

	REFLECT_DATA_TYPE_DEFAULT(MockCustomDataType)
} // namespace nabitest

#endif // #ifdef _DEBUG
