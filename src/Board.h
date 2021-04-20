#ifndef _BOARD_
#define _BOARD_

#include "DataProviders/ConfigDataProvider.h"
#include "Models/Element.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Models/Kernel.h"
#include "ScoreManager.h"

namespace game
{

    struct IBoard
    {
        IBoard() = default;
        virtual ~IBoard() = default;
        
        virtual bool Init() = 0;
        virtual void Draw(sf::RenderWindow& window) = 0;
        virtual void HandleClick(sf::Event click) = 0;
    };

	class Board : public IBoard
	{
	public:
		Board(std::shared_ptr<IConfigDataProvider>,
			std::shared_ptr<IResourceDataProvider>,
			std::shared_ptr<KernelProvider>,
			int x = 0,
			int y = 0);
		~Board();

		bool Init();
		void Draw(sf::RenderWindow& window);
		void HandleClick(sf::Event click);

	private:

		auto _Dealloc() -> void;
		auto _DrawClickInfo(sf::RenderWindow& window) -> void;
		auto _GetElementPosition(sf::Vector2i coord) -> sf::Vector2i;
		auto _ResetCoords() -> void;
		auto _CheckMoveCoords() -> bool;
		auto _Move() -> void;
		auto _RemoveRow(size_t y) -> void;
		auto _RemoveColumn(size_t x) -> void;
        auto _RemoveRect(sf::Vector2i center) -> void;
        auto _CheckBoard() -> bool;
        auto _Split(std::string str, const std::string& token) -> std::vector<std::string>;
		auto _HandleBombClick(sf::Vector2i position) -> void;
		auto _HandleColorClick(sf::Vector2i position) -> void;
		auto _SortColumn() -> void;

	private:
		std::shared_ptr<IConfigDataProvider>	_config_dp{nullptr};
		std::shared_ptr<IResourceDataProvider>	_resource_dp{nullptr};
		std::unique_ptr<ElementFactory>			_factory{nullptr};
		std::unique_ptr<ScoreManager>			_score_manager{ nullptr };
		std::shared_ptr<KernelProvider>			_kernels{nullptr};

		Matrix<Tile*>			                _tiles{};
        Matrix<Element*>		                _board{};

		int										_top_left_x{};
		int										_top_left_y{};
		sf::IntRect								_board_rect{};
		
		//CODE FOR TESTING
		//bool									_in{false};
		//sf::Vector2i							_position{ -1, -1 };
		sf::Vector2i							_position_c{ -1, -1 };
		sf::Vector2i							_position_n{ -1, -1 };
	};
}

#endif // !_BOARD_
