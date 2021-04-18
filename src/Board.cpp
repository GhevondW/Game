#include "Board.h"

using namespace game;

Board::Board(std::shared_ptr<IConfigDataProvider> config, std::shared_ptr<IResourceDataProvider> resource)
	:_config_dp(config),
	_resource_dp(resource)
{}

void Board::Init()
{
	_factory = std::make_unique<ElementFactory>(_resource_dp);
	_Dealloc();

	int rows = _config_dp->GetRow();
	int cols = _config_dp->GetColumn();
	const std::string& config_string = _config_dp->GetBoard();

	if (config_string.empty() || rows * cols != config_string.size()) return;

	_tiles.resize(rows);
	_board.resize(rows);

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
}

void Board::Draw(sf::RenderWindow* window)
{
	const int top_x = 115;
	const int top_y = 250;
	const int elem_padding = 6;

	int rows = _config_dp->GetRow();
	int cols = _config_dp->GetColumn();
	for (size_t y = 0; y < rows; y++)
	{
		for (size_t x = 0; x < cols; x++)
		{
			int xPos = top_x + x * 87;
			int yPos = top_y + y * 87;
			_tiles[y][x]->SetPosition(xPos, yPos);
			//_board[y][x]->SetPosition(xPos + elem_padding, yPos + elem_padding);
			_board[y][x]->SetCenterOfRect(_tiles[y][x]->GetRectPosition());
			window->draw(_tiles[y][x]->GetSprite());
			window->draw(_board[y][x]->GetSprite());
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