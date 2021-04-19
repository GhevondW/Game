#include "ScoreManager.h"

using namespace game;

ScoreManager::ScoreManager(IObjectiveProvider* objectives,
							ElementFactory* facory)
	:_objectives_provider_ref(objectives),
	_factory_ref(facory)
{}

ScoreManager::~ScoreManager()
{
	auto c = _objectives.begin();
	while (c != _objectives.end())
	{
		delete c->second.second;
		c->second.second = nullptr;
		++c;
	}
}

bool ScoreManager::Init()
{
	if (_factory_ref == nullptr || _objectives_provider_ref == nullptr) return false;

	const auto& objectives = _objectives_provider_ref->GetObjectives();
	for (const auto& obj_info : objectives)
	{
		auto type = _factory_ref->GetElemTypeByCode(obj_info.code);
		_objectives[type] = {obj_info.count, _factory_ref->CreateElement(type)};
	}

	std::string path = R_PATH;
	if (!_font.loadFromFile(path + "font.ttf"))
	{
		return false;
	}

	return true;
}

void ScoreManager::Draw(sf::RenderWindow& window)
{
	const int step = 150;
	const int step_number = 35;
	auto c = _objectives.begin();
	int counter = 1;
	while (c != _objectives.end())
	{
		Element* elem = c->second.second;
		elem->SetPosition(counter * step, 10);
		int value = c->second.first;

		sf::Text text;
		text.setFont(_font);
		text.setString(std::to_string(value));
		text.setCharacterSize(40);
		text.setFillColor(sf::Color::Black);
		text.setStyle(sf::Text::Bold);
		text.setPosition(elem->GetRectPosition().left - step_number, elem->GetRectPosition().top + step_number / 4);
		window.draw(text);
		window.draw(elem->GetSprite());
		++counter;
		++c;
	}
}