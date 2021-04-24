#include "IAnimationCommand.h"

using namespace game;

//---------------------------------MoveElementCommand

ElementFallCommand::ElementFallCommand(Element& ref, float y_move, size_t mf)
	:_element_ref(ref),
	_y_move(y_move),
	_move_factor(mf) 
{
	/*_move_step_y = (_end.y - _begin.y) / _move_factor;*/
	_status = ExecutionPhase::Begin;
}

auto ElementFallCommand::Excecute() -> ExecutionPhase
{
	if (_status == ExecutionPhase::Begin) {
		_status = ExecutionPhase::Progress;
		_begin = _element_ref.GetPosition();
		_end = _begin;
		_end.y += _y_move;
		_move_step_y = (_end.y - _begin.y) / _move_factor;
		return _status;
	}

	_status = ExecutionPhase::Progress;
	sf::Vector2f pos_current = _element_ref.GetPosition();
	if (pos_current == _end) {
		_status = ExecutionPhase::End;
		return _status;
	}

	pos_current.y = MIN((pos_current.y + _move_step_y), _end.y);

	_element_ref.SetPosition(pos_current);
	return _status;
}

//---------------------------------ElementMoveCommand

ElementMoveCommand::ElementMoveCommand(game::Element& ref, sf::Vector2f end, size_t mf)
	:_element_ref(ref),
	_end(end),
	_move_factor(mf)
{
	_status = ExecutionPhase::Begin;
}

auto ElementMoveCommand::Excecute() -> ExecutionPhase
{
	if (_status == ExecutionPhase::Begin) {
		_status = ExecutionPhase::Progress;
		_begin = _element_ref.GetPosition();
		_move_step_y = (_end.y - _begin.y) / _move_factor;
		_move_step_x = (_end.x - _begin.x) / _move_factor;
		return _status;
	}

	_status = ExecutionPhase::Progress;
	sf::Vector2f pos_current = _element_ref.GetPosition();
	if (pos_current == _end) {
		_status = ExecutionPhase::End;
		return _status;
	}

	auto new_y = pos_current.y + _move_step_y;
	pos_current.y = _move_step_y < 0 ? MAX(new_y, _end.y) : MIN(new_y, _end.y);
	auto new_x = pos_current.x + _move_step_x;
	pos_current.x = _move_step_x < 0 ? MAX(new_x, _end.x) : MIN(new_x, _end.x);

	_element_ref.SetPosition(pos_current);
	return _status;

}

//---------------------------------ShowElementCommand

ShowElementCommand::ShowElementCommand(game::Element& ref, size_t mf)
	:_element_ref(ref),
	_move_factor(mf)
{
	_position = _element_ref.GetSprite().getTextureRect();
	sf::IntRect copy = _position;

	_height_step = MAX(1, copy.height / mf);
	_width_step = MAX(1, copy.height / mf);

	copy.width = 0;
	copy.height = 0;
	_element_ref.GetSprite().setTextureRect(copy);
	_begin = copy;
}

auto ShowElementCommand::Excecute() -> ExecutionPhase
{
	ExecutionPhase phase = ExecutionPhase::End;
	sf::IntRect pos_current = _element_ref.GetSprite().getTextureRect();
	if (pos_current == _position) return phase;

	if (pos_current == _begin)
		phase = ExecutionPhase::Begin;
	else
		phase = ExecutionPhase::Progress;

	pos_current.width = MIN(pos_current.width + _width_step, _position.width);
	pos_current.height = MIN(pos_current.height + _height_step, _position.height);

	_element_ref.GetSprite().setTextureRect(pos_current);
	return phase;
}