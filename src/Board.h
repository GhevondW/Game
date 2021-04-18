#ifndef _BOARD_
#define _BOARD_

#include "DataProviders/ConfigDataProvider.h"
#include "Models/Element.h"
#include <vector>
#include <SFML/Graphics.hpp>

namespace game
{
	class Board
	{
	public:
		Board(std::shared_ptr<IConfigDataProvider>, std::shared_ptr<IResourceDataProvider>);
		~Board();

		void Init();
		void Draw(sf::RenderWindow* window);

	private:

		void _Dealloc();

	private:
		std::shared_ptr<IConfigDataProvider>	_config_dp{ nullptr };
		std::shared_ptr<IResourceDataProvider>	_resource_dp{ nullptr };
		std::unique_ptr<ElementFactory>			_factory{nullptr};

		std::vector<std::vector<Tile*>>			_tiles{};
		std::vector<std::vector<Element*>>		_board{};
	};
}

#endif // !_BOARD_
