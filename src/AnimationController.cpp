#include "AnimationController.h"
#include <thread>

using namespace game;
using namespace std;

AnimationController::AnimationController(size_t dt)
	:_delay_milliseconds(dt)
{

}

auto AnimationController::AddGroup() -> void
{
	_commands[_groups];
	++_groups;
}

auto AnimationController::AddCommand(ICommand command) -> void
{
	if (_commands.empty()) AddGroup();

	auto iter = _commands.rbegin();
	if (command != nullptr) {
		iter->second.push_back(std::move(command));
	}
}

auto AnimationController::EraseFirsGroup() -> void
{
	if (!_commands.empty()) {
		_commands.erase(_commands.begin());
	}
	if (Empty()) _groups = 0;
}

auto AnimationController::EraseLastGroup() -> void
{
	if (!_commands.empty()) {
		auto last = _commands.end();
		--last;
		_commands.erase(last);
	}
	if (Empty()) _groups = 0;
}

auto AnimationController::GetFirstGroup() const -> const std::deque<ICommand>&
{
	const auto& begin = std::begin(_commands);
	if (begin == end(_commands)) throw "invalid operation";
	return begin->second;
}

auto AnimationController::Excecute() -> void
{
	if (!Empty()) {
		const auto& group = GetFirstGroup();
		ExecutionPhase phase = ExecutionPhase::End;
		for (const auto& iter : group)
		{
			auto res = iter->Excecute();
			if (res == ExecutionPhase::Begin || res == ExecutionPhase::Progress) {
				phase = res;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(GetDelay()));

		if (phase == ExecutionPhase::End) {
			EraseFirsGroup();
		}
	}
}

auto AnimationController::Empty() -> bool
{
	return _commands.empty();
}