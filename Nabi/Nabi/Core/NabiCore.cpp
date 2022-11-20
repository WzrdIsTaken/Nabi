#include "NabiCore.h"

#include "../Reflection/MetaObjectLookup.h"
#include "../Reflection/XmlParser.h"

namespace nabi
{
	entt::registry& NabiCore::GetRegistry()
	{
		return m_Registry;
	}
} // namespace nabi
