#ifndef _RESOURCE_DATA_PROVIDER_H_
#define _RESOURCE_DATA_PROVIDER_H_

#include "Resource.h"

namespace game
{
	struct IResourceDataProvider
	{
		virtual ~IResourceDataProvider() = default;
		virtual Resource ReadResource(const std::string& path) = 0;
	};

	struct JSONResourceDataProvider : public IResourceDataProvider
	{
		Resource ReadResource(const std::string& path) override;
	};

	//struct XMLConfigDataProvider : public IResourceDataProvider
	//{
	//	Resource ReadResource(const std::string& path) override;
	//};
}

#endif // !_RESOURCE_DATA_PROVIDER_H_

