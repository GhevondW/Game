#ifndef _BOARD_
#define _BOARD_

#include "DataProviders/ConfigDataProvider.h"
#include "Models/Element.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Models/Kernel.h"
#include "ScoreManager.h"
#include "IDrawable.h"
#include "AnimationController.h"

namespace game
{

    struct IBoard : public IDrawable
    {
        IBoard() = default;
        virtual ~IBoard() = default;
        
        virtual bool Init() = 0;
        virtual void HandleClick(sf::Event click) = 0;
		virtual void Autoplay() = 0;
    };

	class Board : public IBoard
	{
	public:
		Board(std::shared_ptr<IConfigDataProvider>,
			std::shared_ptr<IElementFactory>,
			std::shared_ptr<IScoreController>,
			std::shared_ptr<KernelProvider>,
			int window_width = 0,
			int window_height = 0);
		~Board();

		bool Init();
		void Draw(sf::RenderWindow& window);
		void HandleClick(sf::Event click);
		void Autoplay() override;

	private:

		auto _Dealloc() -> void;
		auto _GetElementPosition(sf::Vector2i coord) -> sf::Vector2i;
		auto _ResetCoords() -> void;
		auto _CheckMoveCoords() -> bool;
		auto _Move() -> void;
		auto _RemoveRow(size_t y) -> void;
		auto _RemoveColumn(size_t x) -> void;
        auto _RemoveRect(sf::Vector2i center) -> void;
        auto _CheckBoard() -> bool;
        auto _Split(std::string str, const std::string& token) -> std::vector<std::string>;
		auto _HandleBombClick(sf::Vector2i position) -> bool;
		auto _HandleColorClick(sf::Vector2i position) -> bool;
		auto _SortColumn() -> void;
        auto _GenerateNewElements() -> void;
        auto _IsElementBomb(sf::Vector2i point) const -> bool;
		auto _IsElementEmpty(sf::Vector2i point) const -> bool;
		auto _GetPositionInTile(const Element* elem, const Tile* tile) -> sf::Vector2f;

	private:
		std::shared_ptr<IConfigDataProvider>	_config_dp{nullptr};
		std::shared_ptr<IElementFactory>		_factory{nullptr};
		std::shared_ptr<IScoreController>		_score_controller{ nullptr };
		std::shared_ptr<KernelProvider>			_kernels{nullptr};
		std::shared_ptr<AnimationController>	_animations{nullptr};

		Matrix<Tile*>			                _tiles{};
        Matrix<Element*>		                _board{};

		const int								_window_width;
		const int								_window_height;
		sf::IntRect								_board_rect{};
		
		sf::Vector2i							_position_c{ -1, -1 };
		sf::Vector2i							_position_n{ -1, -1 };

		bool									_autoplay{false};
	};
}

#endif // !_BOARD_
