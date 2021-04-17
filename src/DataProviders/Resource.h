#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <vector>

namespace game
{
	struct Color
	{
		std::string name{};
		std::string image_path{};
		std::string code{};
	};

	struct Bomb
	{
		std::string name{};
		std::string image_path{};
		std::string code{};
	};

	struct Tile
	{
		std::string name{};
		std::string image_path{};
		std::string code{};
	};

	struct Resource
	{
		std::vector<Color>	colors{};
		std::vector<Bomb>	bombs{};
		std::vector<Tile>	tiles{};
	};

}

#endif // !_RESOURCE_H_

