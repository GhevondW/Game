#ifndef _RESOURCE_DATA_PROVIDER_H_
#define _RESOURCE_DATA_PROVIDER_H_

#include "Resource.h"
#include "LoadStatus.h"

namespace game
{
	struct IResourceDataProvider
	{
		IResourceDataProvider() = default;
		virtual ~IResourceDataProvider() = default;
		virtual LoadStatus Load(const std::string& path) = 0;

		const std::vector<ElementInfo>& GetElements() const { 
			if (_resource == nullptr) throw "invalid operation";
			return _resource->elements; 
		}
		const std::vector<TileInfo>& GetTiles() const { 
			if (_resource == nullptr) throw "invalid operation";
			return _resource->tiles; 
		}

	protected:
		std::unique_ptr<Resource> _resource{nullptr};
	};

	struct JSONResourceDataProvider : public IResourceDataProvider
	{
		LoadStatus Load(const std::string& path) override;
	};

	//struct XMLConfigDataProvider : public IResourceDataProvider
	//{
	//	Resource ReadResource(const std::string& path) override;
	//};
}

#endif // !_RESOURCE_DATA_PROVIDER_H_

