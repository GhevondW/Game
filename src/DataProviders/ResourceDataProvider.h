#ifndef _RESOURCE_DATA_PROVIDER_H_
#define _RESOURCE_DATA_PROVIDER_H_

#include "Resource.h"

namespace game
{
	struct IResourceDataProvider
	{
		IResourceDataProvider() = default;
		virtual ~IResourceDataProvider() = default;
		virtual const Resource& Load(const std::string& path) = 0;

		const std::vector<ElementInfo>& GetElements() const { return _resource.elements; }
		const std::vector<TileInfo>& GetTiles() const { return _resource.tiles; }

	protected:
		Resource _resource{};
	};

	struct JSONResourceDataProvider : public IResourceDataProvider
	{
		const Resource& Load(const std::string& path) override;
	};

	//struct XMLConfigDataProvider : public IResourceDataProvider
	//{
	//	Resource ReadResource(const std::string& path) override;
	//};
}

#endif // !_RESOURCE_DATA_PROVIDER_H_

