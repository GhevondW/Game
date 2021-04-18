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
		Board(std::shared_ptr<IConfigDataProvider>,
			std::shared_ptr<IResourceDataProvider>,
			int x = 0,
			int y = 0);
		~Board();

		void Init();
		void Draw(sf::RenderWindow& window);
		void HandleClick(sf::Event click);

	private:

		auto _Dealloc() -> void;
		auto _DrawClickInfo(sf::RenderWindow& window) -> void;
		auto _GetElementPosition(sf::Vector2i coord) -> sf::Vector2i;
		auto _ResetCoords() -> void;
		auto _CheckMoveCoords() -> bool;
		auto _Move() -> void;

	private:
		std::shared_ptr<IConfigDataProvider>	_config_dp{ nullptr };
		std::shared_ptr<IResourceDataProvider>	_resource_dp{ nullptr };
		std::unique_ptr<ElementFactory>			_factory{nullptr};

		std::vector<std::vector<Tile*>>			_tiles{};
		std::vector<std::vector<Element*>>		_board{};

		int										_top_left_x{};
		int										_top_left_y{};
		sf::IntRect								_board_rect{};
		
		//CODE FOR TESTING
		//bool									_in{false};
		sf::Vector2i							_position{-1, -1};
		sf::Vector2i							_position_c{ -1, -1 };
		sf::Vector2i							_position_n{ -1, -1 };
	};
}

#endif // !_BOARD_
