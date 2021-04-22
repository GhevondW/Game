#include "ScoreManager.h"
#include "GlobalDefines.h"

using namespace game;

ScoreManager::ScoreManager(IObjectiveProvider* objectives,
							IElementFactory* facory)
	:_objectives_provider_ref(objectives),
	_factory_ref(facory)
{
	if (!_Init()) {
		throw "invalid operation";
	}
}

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

bool ScoreManager::_Init()
{
	if (_factory_ref == nullptr || _objectives_provider_ref == nullptr) return false;
    
    _total_count = 0;
    
	const auto& objectives = _objectives_provider_ref->GetObjectives();
	for (const auto& obj_info : objectives)
	{
		auto type = _factory_ref->GetElemTypeByCode(obj_info.code);
		_objectives[type] = {obj_info.count, _factory_ref->CreateElement(type)};
        _total_count += obj_info.count;
	}

	std::string path = R_PATH;
	if (!_font.loadFromFile(path + "font.ttf"))
	{
		return false;
	}

    _moves_count = _objectives_provider_ref->GetMovesCount();
    
	return true;
}

void ScoreManager::UpdateScore(Element::TYPE type, int count)
{
    if(_objectives.count(type)){
        int old_value = _objectives[type].first;
        _objectives[type].first = MAX(old_value - count, 0);
        
        int ds = old_value - _objectives[type].first;
        _total_count -= ds;
    }
}

auto ScoreManager::UpdateMovesCount() -> bool
{
    _moves_count = MAX(_moves_count - 1, 0);
    return _moves_count == 0;
}

auto ScoreManager::CheckGameStatus() const -> STATUS
{
	if (_total_count <= 0 && _moves_count >= 0) {
		return STATUS::Won;
	}
	else if (_total_count > 0 && _moves_count <= 0) {
		return STATUS::Lose;
	}
	else {
		return STATUS::Game;
	}
}

auto ScoreManager::_CreateText(const std::string& str, size_t size) const -> sf::Text
{
    sf::Text text;
    text.setFont(_font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    return text;
}

void ScoreManager::Draw(sf::RenderWindow& window)
{
	const int step = 150;
	const int step_number = 40;
	auto c = _objectives.begin();
	int counter = 1;

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(window.getSize().x, 100));
	rectangle.setFillColor(sf::Color{156,214,214,255});
	rectangle.setPosition(0, 5);
	
	window.draw(rectangle);

	while (c != _objectives.end())
	{
		Element* elem = c->second.second;
		elem->SetPosition(counter * step, 20);
		int value = c->second.first;
		sf::Text text = _CreateText(std::to_string(value), 35);
		text.setPosition(elem->GetRectPosition().left - step_number, elem->GetRectPosition().top + step_number / 2);
		window.draw(text);
		window.draw(elem->GetSprite());
		++counter;
		++c;
	}
    
    sf::Text text = _CreateText(std::to_string(_moves_count), 50);
    text.setPosition(650, 20);
    window.draw(text);
}
