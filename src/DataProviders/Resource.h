#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <vector>

namespace game
{
	struct ElementInfo
	{
		std::string name{};
		std::string image_path{};
		std::string code{};
	};

	struct TileInfo
	{
		std::string name{};
		std::string image_path{};
		std::string code{};
	};

	struct Resource
	{
		std::vector<ElementInfo>	elements{};
		std::vector<TileInfo>		tiles{};
	};

}

#endif // !_RESOURCE_H_

