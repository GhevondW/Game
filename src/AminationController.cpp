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
	//std::lock_guard<std::mutex> lg(_mtx);
	_commands[_groups];
	++_groups;
}

auto AnimationController::CheckLastGroup() -> void
{
	if (Empty()) return;
	auto end = std::end(_commands);
	--end;
	if (end->second.empty()) {
		_commands.erase(end);
	}
}

auto AnimationController::AddCommand(ICommand command) -> void
{
	if (Empty()) throw "invalid operation";//AddGroup();

	//std::lock_guard<std::mutex> lg{_mtx};
	auto iter = _commands.rbegin();
	if (command != nullptr) {
		iter->second.push_back(std::move(command));
	}
}

auto AnimationController::EraseFirsGroup() -> void
{
	//std::lock_guard<std::mutex> lg{ _mtx };
	if (!_commands.empty()) {
		_commands.erase(_commands.begin());
	}
	if (Empty()) _groups = 0;
}

auto AnimationController::EraseLastGroup() -> void
{
	//std::lock_guard<std::mutex> lg{ _mtx };
	if (!_commands.empty()) {
		auto last = _commands.end();
		--last;
		_commands.erase(last);
	}
	if (_commands.empty()) _groups = 0;
}

auto AnimationController::GetFirstGroup() const -> const std::deque<ICommand>&
{
	const auto& begin = std::begin(_commands);
	if (begin == end(_commands)) throw "invalid operation";
	return begin->second;
}

auto AnimationController::Excecute() -> void
{
	//std::lock_guard<std::mutex> lg{ _mtx };
	if (!_commands.empty()) {
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
			if (!_commands.empty()) {
				_commands.erase(_commands.begin());
			}
		}
	}
}

auto AnimationController::Empty() -> bool
{
	//std::lock_guard<std::mutex> lg{_mtx};
	return _commands.empty();
}