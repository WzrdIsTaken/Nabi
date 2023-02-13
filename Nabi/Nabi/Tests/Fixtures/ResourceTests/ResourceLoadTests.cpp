#include "TestCore.h"

#include "Context.h"
#include "Resource.h"
#include "ResourceBank.h"

#ifdef RUN_TESTS

namespace nabitest::ResourceTests
{
	struct MockResource
	{
		int m_Member;
	};

	class MockResourceLoader
	{
	public:
		typedef std::shared_ptr<MockResource> ResourceType;

		ResourceType operator()(std::string const& /*resourcePath*/, nabi::Context const& /*context*/)
		{
			// A real loader would probs do something with the disk path, and perhaps use the context to eg access the RenderCommand

			ResourceType resource = std::make_shared<MockResource>();
			resource->m_Member = 5;

			return resource;
		}
	};

	// Checks that a resource can be loaded
	TEST(ResourceTests, LoadResource)
	{
		using namespace nabi;
		using namespace nabi::Resource;

		Context context;
		ResourceBank<MockResource, MockResourceLoader, 1u> resourceBank(context);

		std::string const mockPath = "SomeOnDiskPath";
		ResourceRef<MockResource> resourceRef = resourceBank.LoadResource(mockPath);

		int const resourceMemberValue = resourceRef.GetResource()->m_Member;
		Comparison<int> const resourceValue(5, resourceMemberValue);

		COMPAIR_EQ(resourceValue);
	}

	// Checks a resource can be removed
	TEST(ResourceTests, RemoveResource)
	{
		using namespace nabi;
		using namespace nabi::Resource;

		Context context;
		ResourceBank<MockResource, MockResourceLoader, 1u> resourceBank(context);

		std::string const mockPath = "SomeOnDiskPath";
		ResourceRef<MockResource> resourceRef = resourceBank.LoadResource(mockPath);

		// Single resource remove
		size_t const resourceIndex = static_cast<size_t>(resourceRef.m_Id);
		resourceBank.RemoveResource(resourceIndex);

		Comparison<bool> const resourceExists(false, resourceBank.CheckIfResourceIsValid(resourceIndex));
		COMPAIR_EQ(resourceExists);
		
		// Clear (remove all resources)
		resourceRef = resourceBank.LoadResource(mockPath, ResourceBankInsertionMode::Enum::FirstNullIndex);
		resourceBank.Clear();

		size_t const activeResourcesInBank = resourceBank.GetResourceCount();
		Comparison<int> const activeResources(0, activeResourcesInBank);
		COMPAIR_EQ(activeResources);
	}
} // namespace nabitest::ResourceTests

#endif // #ifdef RUN_TESTS
