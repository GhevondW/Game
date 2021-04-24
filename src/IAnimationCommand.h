#ifndef _I_ANIMATION_COMMAND_H_
#define _I_ANIMATION_COMMAND_H_

#include <SFML/Graphics.hpp>
#include "Models/Element.h"
#include "GlobalDefines.h"

namespace game
{

	enum ExecutionPhase
	{
		Begin,
		Progress,
		End
	};

	struct IAnimationCommand
	{
		IAnimationCommand() = default;
		virtual ~IAnimationCommand() = default;

		virtual auto Excecute() ->ExecutionPhase = 0;
	};

	struct ElementFallCommand : public IAnimationCommand
	{
		ElementFallCommand(game::Element& ref, float y_stet, size_t mf);
		auto Excecute()->ExecutionPhase override;

	private:
		Element&		_element_ref;
		float			_y_move{};
		size_t			_move_factor{};
		ExecutionPhase	_status{ ExecutionPhase::Begin };

		sf::Vector2f	_begin{};
		sf::Vector2f	_end{};
		float			_move_step_y{};
	};

	struct ElementMoveCommand : public IAnimationCommand
	{
		ElementMoveCommand(game::Element& ref, sf::Vector2f end, size_t mf);
		auto Excecute()->ExecutionPhase override;

	private:
		Element&		_element_ref;
		size_t			_move_factor{};
		ExecutionPhase	_status{ ExecutionPhase::Begin };

		sf::Vector2f	_begin{};
		sf::Vector2f	_end{};
		float			_move_step_y{};
		float			_move_step_x{};
	};

	struct ShowElementCommand : public IAnimationCommand
	{
		ShowElementCommand(game::Element& ref, size_t mf);
		auto Excecute()->ExecutionPhase override;

	private:
		Element&		_element_ref;
		size_t			_move_factor{};

		sf::IntRect		_position{};
		sf::IntRect		_begin{};
		short			_width_step{};
		short			_height_step{};
	};

}

#endif // !_I_ANIMATION_COMMAND_H_
