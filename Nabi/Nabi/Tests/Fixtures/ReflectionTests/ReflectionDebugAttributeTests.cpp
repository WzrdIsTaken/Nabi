#include "../../TestCore.h"

#include "../../../Libraries/pugixml/pugixml.hpp"

#include "../../../Reflection/XmlParser.h"
#include "../../../Utils/BuildUtils.h"

// This is kinda jank test ngl, I gotta get better at writing these things...

#ifdef RUN_TESTS

namespace nabitest::ReflectionTests
{
#ifdef _DEBUG // Need _DEBUG because thats back XmlParser::CheckIfNodeHasDebugPropertyAndConfigurationIsDebug uses
#ifdef USE_DEBUG_UTILS
	// Checks that debug attribute works as expected
	TEST(RelfectionTests, ParseEntityWithDebugAttribute)
	{
		using namespace nabi::Utils::BuildUtils;

		// Mock objects
		entt::registry registry;
		std::string const docPath = "Tests/TestData/TestReflectionData/test_debug_entity_file.xml";
		nabi::Reflection::XmlParser xmlParser{};

		// Release mode test
		c_BuildConfiguration = BuildConfiguration::Release;

		pugi::xml_document doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, registry);

		Comparison<size_t> entityCount(0, registry.size());
		COMPAIR_EQ(entityCount);

		registry.clear();

		// Debug mode test
		c_BuildConfiguration = BuildConfiguration::Debug;

		doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, registry);

		entityCount = Comparison<size_t>(1, registry.size());
		COMPAIR_EQ(entityCount);

		registry.clear();

		// Double check that c_BuildConfiguration is Debug at the end (otherwise this could cause some odd bugs xD)!!
		Comparison<BuildConfiguration> correctBuildConfig(BuildConfiguration::Debug, c_BuildConfiguration);
		COMPAIR_EQ(correctBuildConfig);
	}
#endif // ifdef USE_DEBUG_UTILS
#endif // ifdef _DEBUG
} // namespace nabitest::ReflectionTests

#endif // #ifdef RUN_TESTS
