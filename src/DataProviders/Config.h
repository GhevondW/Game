#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <vector>
namespace game {
	struct Objective
	{
		std::string color{};
		size_t		count{};
	};

	struct Config
	{
		size_t					row{};
		size_t					column{};
		size_t					moves_count{};
		size_t					fig_colors_count{};
		std::vector<Objective>	objectives{};
		std::string				board{};
	};
}

#endif // !_CONFIG_H_

