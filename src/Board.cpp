#include "Board.h"
#include "Models/Kernel.h"
#include <set>

using namespace game;

Board::Board(std::shared_ptr<IConfigDataProvider> config,
	std::shared_ptr<IResourceDataProvider> resource,
	std::shared_ptr<KernelProvider> kernels,
	int x, int y)
	:_config_dp(config),
	_resource_dp(resource),
	_kernels(kernels),
	_top_left_x(x),
	_top_left_y(y)
{}

void Board::Init()
{
	_factory = std::make_unique<ElementFactory>(_resource_dp);
	_Dealloc();

	int rows = _config_dp->GetRow();
	int cols = _config_dp->GetColumn();
	const std::string& config_string = _config_dp->GetBoard();

	if (config_string.empty() || rows * cols > config_string.size()) return;

	_tiles.resize(rows);
	_board.resize(rows);

	_board_rect.left = _top_left_x;
	_board_rect.top = _top_left_y;
	_board_rect.width = cols * TILE_SIZE;
	_board_rect.height = rows * TILE_SIZE;

	for (size_t y = 0; y < rows; y++)
	{
		int row_begin_color = y % 2;
		_tiles[y].resize(cols);
		_board[y].resize(cols);
		for (size_t x = 0; x < cols; x++)
		{
			int color = (x % 2);
			color = row_begin_color ? row_begin_color - color : color;
			_tiles[y][x] = _factory->CreateTile(Tile::TYPE(color));


			int str_index = y * cols + x;
			auto type = _factory->GetElemTypeByCode(std::string{ config_string[str_index] });
			_board[y][x] = _factory->CreateElement(type);
		}
	}

	_score_manager = std::make_unique<ScoreManager>(_config_dp.get(), _factory.get());
	_score_manager->Init();
}

void Board::HandleClick(sf::Event click)
{
	if (click.type == sf::Event::MouseButtonPressed)
	{
		if (click.mouseButton.button == sf::Mouse::Left)
		{
			int x = click.mouseButton.x;
			int y = click.mouseButton.y;
			//_in = _board_rect.contains({_x, _y});
			_position = _GetElementPosition({x, y});

			if (_position.x != -1) {
				if (_position_c.x == -1 || (_position_c.x != -1 && _position_n.x != -1)) {
					_position_c = _position;
					_position_n = { -1, -1 };
				}
				else if(_position_n.x == -1){
					_position_n = _position;
				}
			}
			else {
				_ResetCoords();
			}

			if (_position_c.x != -1 && _position_n.x != -1) {
				if (_CheckMoveCoords()) {
					_Move();
					_CheckBoard();
				}
			}
		}
	}
}

void Board::Draw(sf::RenderWindow& window)
{
	const int top_x = _board_rect.left;
	const int top_y = _board_rect.top;

	int rows = _config_dp->GetRow();
	int cols = _config_dp->GetColumn();
	for (size_t y = 0; y < rows; y++)
	{
		for (size_t x = 0; x < cols; x++)
		{
			int xPos = top_x + x * TILE_SIZE;
			int yPos = top_y + y * TILE_SIZE;
			_tiles[y][x]->SetPosition(xPos, yPos);
			window.draw(_tiles[y][x]->GetSprite());
			if (_board[y][x]) {
				_board[y][x]->SetCenterOfRect(_tiles[y][x]->GetRectPosition());
				window.draw(_board[y][x]->GetSprite());
			}
		}
	}
	_DrawClickInfo(window);

	_score_manager->Draw(window);
}

void Board::_DrawClickInfo(sf::RenderWindow& window)
{
	sf::Font MyFont;
	std::string path = R_PATH;
	if (!MyFont.loadFromFile(path + "font.ttf"))
	{
		// Error...
	}
	sf::Text text;
	text.setFont(MyFont);
	text.setString("x:" + std::to_string(_position.x) +
		", y:" + std::to_string(_position.y) +
		", Status:" + std::to_string(_position.x != -1) + 
		", x_c:" + std::to_string(_position_c.x) +
		", y_c:" + std::to_string(_position_c.y) + 
		", x_n:" + std::to_string(_position_n.x) + 
		", y_n:" + std::to_string(_position_n.y));

	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	text.setPosition({ 50,100 });
	window.draw(text);
}

auto Board::_GetElementPosition(sf::Vector2i coord) -> sf::Vector2i
{
	sf::Vector2i ret{-1, -1};
	if (_board_rect.contains(coord)) {
		ret.x = (coord.x - _board_rect.left) / TILE_SIZE;
		ret.y = (coord.y - _board_rect.top) / TILE_SIZE;
	}
	return ret;
}

auto Board::_ResetCoords() -> void
{
	_position_c = { -1 , -1};
	_position_n = { -1 , -1};
}

auto Board::_CheckMoveCoords() -> bool
{
    return ((_position_c.x == _position_n.x) && ((_position_c.y == _position_n.y - 1) || (_position_c.y == _position_n.y + 1))) ||
    ((_position_c.y == _position_n.y) && ((_position_c.x == _position_n.x - 1) || (_position_c.x == _position_n.x + 1)));
}

auto Board::_Move() -> void
{
	auto* tmp = _board[_position_c.y][_position_c.x];
	_board[_position_c.y][_position_c.x] = _board[_position_n.y][_position_n.x];
	_board[_position_n.y][_position_n.x] = tmp;
}

auto Board::_RemoveRow(size_t y) -> void
{
	int rows = _config_dp->GetRow();
	if (y < rows) {
		for (int x = 0; x < _config_dp->GetColumn(); x++)
		{
			delete _board[y][x];
			_board[y][x] = nullptr;
		}
	}
}

auto Board::_RemoveColumn(size_t x) -> void
{
	int cols = _config_dp->GetColumn();
	if (x < cols) {
		for (int y = 0; y < _config_dp->GetRow(); y++)
		{
			delete _board[y][x];
			_board[y][x] = nullptr;
		}
	}
}

auto Board::_CheckBoard() -> void
{   
	const auto& kernels = _kernels->Get();

    size_t rows = _config_dp->GetRow();
    size_t cols = _config_dp->GetColumn();
    
	auto current = kernels.rbegin();
    while (current != kernels.rend()) {
        const Kernel& kernel = *current;
        ++current;
    
        size_t kernel_row = kernel.GetRows();
        size_t kernel_cols = kernel.GetCols();
        
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                Element::TYPE type = _board[y][x]->GetType();
                
                if(type == Element::TYPE::EMPTY) continue;
                
                std::vector<std::pair<int, int>> same{};
                bool result = false;
                
                if(y + kernel_row - 1 < rows && x + kernel_cols - 1 < cols){
                    for (int ky = 0; ky < kernel_row; ++ky) {
                        for (int kx = 0; kx < kernel_cols; ++kx) {
                            if(kernel.At(ky, kx) == 1 && _board[y + ky][x + kx]->GetType() == type){
                                same.push_back({y + ky, x + kx});
                            }
                        }
                    }
                    result = same.size() == kernel.GetRank();
                }
                
                if(result){
                    for (auto iter : same) {
                        delete _board[iter.first][iter.second];
                        _board[iter.first][iter.second] = _factory->CreateElement(Element::TYPE::EMPTY);
                    }
                }
                
            }
        }
    }
}

void Board::_Dealloc()
{
	for (size_t i = 0; i < _board.size(); i++)
	{
		for (size_t j = 0; j < _board[i].size(); j++)
		{
			delete _board[i][j];
			delete _tiles[i][j];
		}
	}
	_board.clear();
	_tiles.clear();
}

Board::~Board()
{
	_Dealloc();
}
