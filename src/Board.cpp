#include "Board.h"
#include "Models/Kernel.h"
#include <set>
#include <thread>
#include <iostream>

using namespace game;

Board::Board(std::shared_ptr<IConfigDataProvider> config,
	std::shared_ptr<IElementFactory> factory,
	std::shared_ptr<IScoreController> score_controller,
	std::shared_ptr<KernelProvider> kernels,
	int window_width,
	int window_height)
	:_config_dp(config),
	_factory(factory),
	_score_controller(score_controller),
	_kernels(kernels),
	_window_width(window_width),
	_window_height(window_height),
	_animations(std::make_shared<AnimationController>(DELAY_TIME))
{}

bool Board::Init()
{
	_Dealloc();

	int rows = _config_dp->GetRow();
	int cols = _config_dp->GetColumn();
	const std::string& config_string = _config_dp->GetBoard();

	if (config_string.empty()) return false;

	_tiles.resize(rows);
	_board.resize(rows);

	_board_rect.width = cols * TILE_SIZE;
	_board_rect.height = rows * TILE_SIZE;

	int ow = _window_width;
	int oh = _window_height;
	int cw = _board_rect.width;
	int ch = _board_rect.height;
	if (cw < ow && ch < oh) {
		_board_rect.left = (ow - cw) / 2;
		_board_rect.top = (oh - ch) / 2;
	}

    auto tokens = _Split(config_string, "-");
    
    if(tokens.size() < rows * cols) return false;
    
	int top_x = _board_rect.left;
	int top_y = _board_rect.top;

	_animations->AddGroup();

	for (size_t y = 0; y < rows; y++)
	{
		int row_begin_color = y % 2;
		_tiles[y].resize(cols);
		_board[y].resize(cols);
		for (size_t x = 0; x < cols; x++)
		{

			int xPos = top_x + x * TILE_SIZE;
			int yPos = top_y + y * TILE_SIZE;

			int color = (x % 2);
			color = row_begin_color ? row_begin_color - color : color;
			_tiles[y][x] = _factory->CreateTile(Tile::TYPE(color));
			_tiles[y][x]->SetPosition(xPos, yPos);


			int str_index = y * cols + x;
			auto type = _factory->GetElemTypeByCode(tokens[str_index]);
			_board[y][x] = _factory->CreateElement(type);
			_board[y][x]->SetCenterOfRect(_tiles[y][x]->GetRectPosition());

			_animations->AddCommand(ICommand(new ShowElementCommand(*_board[y][x], MOVE_FACTOR)));
		}
	}

	_animations->CheckLastGroup();

	return _score_controller != nullptr;
}

void Board::HandleClick(sf::Event click)
{
	if (!_animations->Empty() && _autoplay == false) return;
	if (click.type == sf::Event::MouseButtonPressed)
	{
        int x = click.mouseButton.x;
        int y = click.mouseButton.y;
        auto position = _GetElementPosition({x, y});
        bool result = false;
		if (click.mouseButton.button == sf::Mouse::Left)
		{
            result = _HandleColorClick(position);
		}
        else if(click.mouseButton.button == sf::Mouse::Right){
            result = _HandleBombClick(position);
        }

		if (result) {
			_GenerateNewElements();
		}
	}
}

void Board::Autoplay()
{
	if (_autoplay == true && _animations->Empty()) {
		if (_CheckBoard()) {
			_GenerateNewElements();
		}
		else {
			_autoplay = false;
		}
	}
}

auto Board::_HandleColorClick(sf::Vector2i position) -> bool
{
	if (position.x != -1 && position.y != -1) {
		if (_IsElementBomb(position) || _IsElementEmpty(position))
		{
			_ResetCoords();
			return false;
		}

		if (_position_c.x == -1 || (_position_c.x != -1 && _position_n.x != -1)) {
			_position_c = position;
			_position_n = { -1, -1 };
		}
		else if (_position_n.x == -1) {
			_position_n = position;
			if (_position_n == _position_c) {
				_ResetCoords();
			}
		}
	}
	else {
		_ResetCoords();
		return false;
	}

	if (_position_c.x != -1 && _position_c.y != -1 && _position_n.x != -1 && _position_n.y != -1) {
		if (_CheckMoveCoords()) {
			if (_IsElementBomb(_position_c) && _IsElementBomb(_position_n))
			{
				_ResetCoords();
				return false;
			}

			_Move();
			if (!_CheckBoard())
			{
				_Move();
				_ResetCoords();
			}
			else {
				_score_controller->UpdateMovesCount();
				_ResetCoords();
				return true;
			}
		}
		else {
			_ResetCoords();
		}
	}

	return false;
}

auto Board::_HandleBombClick(sf::Vector2i position) -> bool
{
	if (position.x != -1 && position.y != -1) {
		Element* element = _board[position.y][position.x];
		auto type = element->GetType();

		bool do_move = false;

		if (type == Element::TYPE::VBOMB) {
			_RemoveColumn(position.x);
			do_move = true;
		}
		else if (type == Element::TYPE::HBOMB) {
			_RemoveRow(position.y);
			do_move = true;
		}
		else if (type == Element::BOMB) {
			_RemoveRect(position);
			do_move = true;
		}

		//if (do_move) {
		//	_score_controller->UpdateMovesCount();
		//	_SortColumn();
		//	//_CheckBoard();
  //          //_GenerateNewElements();
  //          _ResetCoords();
		//}
        
        return do_move;
	}
}

auto Board::_SortColumn() -> void
{
	const size_t rows = _config_dp->GetRow();
	const size_t cols = _config_dp->GetColumn();

	auto Task = [=](int begin_x, int end_x) 
	{
		for (int xPos = begin_x; xPos < end_x; xPos++)
		{
			if (xPos < cols) {
				for (int y = rows - 1; y >= 0; --y)
				{
					if (_board[y][xPos]->GetType() != Element::TYPE::EMPTY) {
						int y_initial = y;
						int x_initial = xPos;

						int y_step = y;
						while (y_step + 1 < rows && _board[y_step + 1][xPos]->GetType() == Element::TYPE::EMPTY)
						{
							Element* tmp = _board[y_step][xPos];
							_board[y_step][xPos] = _board[y_step + 1][xPos];
							_board[y_step + 1][xPos] = tmp;
							//_board[y_step + 1][xPos]->SetCenterOfRect(_tiles[y_step + 1][xPos]->GetRectPosition());
							++y_step;
						}

						if (y_step > y_initial && y_step < rows) {
							Tile* tile = _tiles[y_step][xPos];
							Element* elem = _board[y_step][xPos];
							auto new_pos = _GetPositionInTile(elem, tile);
							_animations->AddCommand(ICommand(new ElementMoveCommand(*elem, new_pos, MOVE_FACTOR)));
						}
					}
				}
			}
		}
	};

	_animations->AddGroup();

	if (/*cols < 4*/ true) {
		Task(0, cols);
	}
	else {
		std::thread t1{Task, 0, cols / 2 };
		std::thread t2{ Task, cols / 2, cols };
		t1.join();
		t2.join();
	}

	_animations->CheckLastGroup();

}

auto Board::_GenerateNewElements() -> void
{
	_animations->AddGroup();
    const size_t rows = _config_dp->GetRow();
    const size_t cols = _config_dp->GetColumn();
    const size_t max = _config_dp->GetFigColorsCount();

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if(_board[y][x] && _board[y][x]->GetType() == Element::TYPE::EMPTY){
                delete _board[y][x];
                _board[y][x] = nullptr;
                _board[y][x] = _factory->CreateRandomElement(max - 1);;
				_board[y][x]->SetCenterOfRect(_tiles[y][x]->GetRectPosition());

				Element* elem = _board[y][x];
				//Tile* tile = _tiles[y][x];

				_animations->AddCommand(ICommand(new ShowElementCommand(*elem, MOVE_FACTOR)));
				_autoplay = true;
            }
        }
    }
	_animations->CheckLastGroup();
    //_CheckBoard();
}

auto Board::_IsElementBomb(sf::Vector2i point) const -> bool
{
    return _board[point.y][point.x]->GetType() == Element::TYPE::BOMB ||
    _board[point.y][point.x]->GetType() == Element::TYPE::VBOMB ||
    _board[point.y][point.x]->GetType() == Element::TYPE::HBOMB;
}

auto Board::_IsElementEmpty(sf::Vector2i point) const -> bool
{
	return _board[point.y][point.x]->GetType() == Element::TYPE::EMPTY;
}

void Board::Draw(sf::RenderWindow& window)
{
	auto size = window.getSize();

	//int top_x = _board_rect.left;
	//int top_y = _board_rect.top;

	int rows = _config_dp->GetRow();
	int cols = _config_dp->GetColumn();

	for (size_t y = 0; y < rows; y++)
	{
		for (size_t x = 0; x < cols; x++)
		{
            if(_position_c.x == x && _position_c.y == y){
                _tiles[y][x]->SetTexture(_factory->GetTextureSelectedTile(_tiles[y][x]->GetType()));
            }

            if(_position_c.x == -1 && _position_c.y == -1){
                _tiles[y][x]->SetTexture(_factory->GetTextureTile(_tiles[y][x]->GetType()));
            }
            
			//int xPos = top_x + x * TILE_SIZE;
			//int yPos = top_y + y * TILE_SIZE;
			//_tiles[y][x]->SetPosition(xPos, yPos);
			window.draw(_tiles[y][x]->GetSprite());
			if (_board[y][x]) {
				//_board[y][x]->SetCenterOfRect(_tiles[y][x]->GetRectPosition());
				window.draw(_board[y][x]->GetSprite());
			}
		}
	}

	_animations->Excecute();

}

//void Board::_DrawClickInfo(sf::RenderWindow& window)
//{
//	sf::Font MyFont;
//	std::string path = R_PATH;
//	if (!MyFont.loadFromFile(path + "font.ttf"))
//	{
//		// Error...
//	}
//	sf::Text text;
//	text.setFont(MyFont);
//	text.setString(/*"x:" + std::to_string(_position.x) +
//		", y:" + std::to_string(_position.y) +
//		", Status:" + std::to_string(_position.x != -1) + */
//		", x_c:" + std::to_string(_position_c.x) +
//		", y_c:" + std::to_string(_position_c.y) + 
//		", x_n:" + std::to_string(_position_n.x) + 
//		", y_n:" + std::to_string(_position_n.y));
//
//	text.setCharacterSize(24);
//	text.setFillColor(sf::Color::Black);
//	text.setStyle(sf::Text::Bold);
//	text.setPosition({ 50,100 });
//	window.draw(text);
//}

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

	//auto pos_n = _GetPositionInTile(_board[_position_n.y][_position_n.x], _tiles[_position_n.y][_position_n.x]);
	//auto pos_c = _GetPositionInTile(_board[_position_c.y][_position_c.x], _tiles[_position_c.y][_position_c.x]);

	_board[_position_c.y][_position_c.x]->SetCenterOfRect(_tiles[_position_c.y][_position_c.x]->GetRectPosition());
	_board[_position_n.y][_position_n.x]->SetCenterOfRect(_tiles[_position_n.y][_position_n.x]->GetRectPosition());
}

auto Board::_RemoveRow(size_t y) -> void
{
	int rows = _config_dp->GetRow();
	if (y < rows) {
		for (int x = 0; x < _config_dp->GetColumn(); x++)
		{
            auto type = _board[y][x]->GetType();
            _score_controller->UpdateScore(type, 1);
            
			delete _board[y][x];
			_board[y][x] = _factory->CreateElement(Element::TYPE::EMPTY);
		}
	}

	_score_controller->UpdateMovesCount();
	_SortColumn();
	_ResetCoords();
}

auto Board::_RemoveColumn(size_t x) -> void
{
	int cols = _config_dp->GetColumn();
	if (x < cols) {
		for (int y = 0; y < _config_dp->GetRow(); y++)
		{
            auto type = _board[y][x]->GetType();
			_score_controller->UpdateScore(type, 1);
            
			delete _board[y][x];
            _board[y][x] = _factory->CreateElement(Element::TYPE::EMPTY);
		}
	}

	_score_controller->UpdateMovesCount();
	_SortColumn();
	_ResetCoords();
}

auto Board::_RemoveRect(sf::Vector2i center) -> void
{
    if(center.x != -1 && center.y != -1) {
        int begin_x = MAX(center.x - 1, 0);
        int begin_y = MAX(center.y - 1, 0);
        int end_x = MIN(center.x + 1, _config_dp->GetColumn() - 1);
        int end_y = MIN(center.y + 1, _config_dp->GetRow() - 1);
        for (int y = begin_y; y <= end_y; y++) {
            for (int x = begin_x; x <= end_x; x++) {
                auto type = _board[y][x]->GetType();
				_score_controller->UpdateScore(type, 1);
                
                delete _board[y][x];
                _board[y][x] = _factory->CreateElement(Element::TYPE::EMPTY);
            }
        }
    }

	_score_controller->UpdateMovesCount();
	_SortColumn();
	_ResetCoords();
}

auto Board::_CheckBoard() -> bool
{   
	const auto& kernels = _kernels->Get();
    bool move_status = false;
    size_t rows = _config_dp->GetRow();
    size_t cols = _config_dp->GetColumn();
    
	auto current = kernels.rbegin();
    while (current != kernels.rend()) {
        const Kernel& kernel = *current;
    
        size_t kernel_row = kernel.GetRows();
        size_t kernel_cols = kernel.GetCols();
        
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                Element::TYPE type = _board[y][x]->GetType();
                
                if(_IsElementBomb({x, y})) continue;
                
                if(type == Element::TYPE::EMPTY) continue;
                
                std::set<std::pair<int, int>> same{};
                bool result = false;
                
                if(y + kernel_row - 1 < rows && x + kernel_cols - 1 < cols){
                    for (int ky = 0; ky < kernel_row; ++ky) {
                        for (int kx = 0; kx < kernel_cols; ++kx) {
                            if(kernel.At(ky, kx) == 1 && _board[y + ky][x + kx]->GetType() == type){
                                same.insert({y + ky, x + kx});
                            }
                        }
                    }
                    result = same.size() == kernel.GetRank();
                }
                
                if(result){
                    move_status = true;
					_score_controller->UpdateScore(type, kernel.GetRank());
					auto iter = same.find({ _position_c.y, _position_c.x });
					iter = iter == end(same) ? same.find({ _position_n.y, _position_n.x }) : iter;
					iter = iter == end(same) ? iter = begin(same) : iter;

					auto c = begin(same);
					while (c != end(same))
					{
						delete _board[c->first][c->second];
						Element::TYPE res_type = Element::TYPE::EMPTY;
						if(kernel.GetRewardType() != Element::TYPE::EMPTY && c == iter){
							res_type = kernel.GetRewardType();
						}
						_board[c->first][c->second] = _factory->CreateElement(res_type);
						_board[c->first][c->second]->SetCenterOfRect(_tiles[c->first][c->second]->GetRectPosition());
						++c;
					}
                }
                
            }
        }
        
        ++current;
    }
    
    if(move_status)
    {
		_SortColumn();
		//_CheckBoard();
        //_GenerateNewElements();
    }
    
    return move_status;
}

auto Board::_Split(std::string str, const std::string& token) -> std::vector<std::string>
{
    using namespace std;
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}
    
void Board::_Dealloc()
{
	for (size_t i = 0; i < _board.size(); i++)
	{
		for (size_t j = 0; j < _board[i].size(); j++)
		{
			delete _board[i][j];
			delete _tiles[i][j];
            _board[i][j] = nullptr;
            _tiles[i][j] = nullptr;
		}
	}
	_board.clear();
	_tiles.clear();
}

auto Board::_GetPositionInTile(const Element* elem, const Tile* tile)->sf::Vector2f
{
	sf::Vector2f res;
	if (elem == nullptr || tile == nullptr) return res;

	auto current_rec = elem->GetRectPosition();
	auto position = tile->GetRectPosition();
	int cw = current_rec.width;
	int ch = current_rec.height;
	int ow = position.width;
	int oh = position.height;
	if (cw < ow && ch < oh) {
		int dw = (ow - cw) / 2;
		int dh = (oh - ch) / 2;
		res.x = position.left + dw;
		res.y = position.top + dh;
	}

	return res;
}

Board::~Board()
{
	_Dealloc();
}
