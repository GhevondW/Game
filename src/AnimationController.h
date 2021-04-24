#ifndef _ANIMATION_CONTROLLER_H_
#define _ANIMATION_CONTROLLER_H_

#include "IAnimationCommand.h"
#include <map>
#include <deque>
#include <memory>
#include <mutex>

namespace game
{

	using ICommand = std::unique_ptr<IAnimationCommand>;

	class AnimationController
	{
	public:
		AnimationController(size_t dt = 0);
		~AnimationController() = default;

		auto GetDelay() const -> size_t { return _delay_milliseconds; }
		auto SetDelay(size_t val) -> void { _delay_milliseconds = val; }

		auto Empty() -> bool;
		auto AddGroup() -> void;
		auto CheckLastGroup() -> void;
		auto AddCommand(ICommand command) -> void;
		auto EraseFirsGroup() -> void;
		auto EraseLastGroup() -> void;
		auto GetFirstGroup() const -> const std::deque<ICommand>&;
		auto Excecute() -> void;

	private:
		size_t								_delay_milliseconds{0};
		size_t								_groups{0};
		std::map<int, std::deque<ICommand>>	_commands{};
		//mutable std::mutex					_mtx{};
	};

}

#endif // !_ANIMATION_CONTROLLER_H_
