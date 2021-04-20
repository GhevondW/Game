#ifndef _SCORE_MANAGER_H_
#define _SCORE_MANAGER_H_

#include "DataProviders/ConfigDataProvider.h"
#include "Models/Element.h"

namespace game
{
	class ScoreManager
	{
	public:
		ScoreManager(IObjectiveProvider* objectives,
					ElementFactory* factory);
		~ScoreManager();

		auto Init() -> bool;
		auto Draw(sf::RenderWindow& window) -> void;
		auto GetObjectivesMap() const -> const std::map<Element::TYPE, std::pair<int, Element*>>& { return _objectives; }
        auto UpdateScore(Element::TYPE, int count) -> void;
        auto UpdateMovesCount() -> bool;
        auto CheckGameStatus() const -> bool { return _total_count == 0; }

    private:
        
        auto _CreateText(const std::string& text, size_t) const -> sf::Text;
        
	private:
		IObjectiveProvider*									_objectives_provider_ref{nullptr};
		ElementFactory*										_factory_ref{nullptr};
		std::map<Element::TYPE, std::pair<int, Element*>>	_objectives{};
		sf::Font											_font{};
        
        int                                                 _total_count{};
        int                                                 _moves_count{};
	};
}

#endif // !_SCORE_MANAGER_H_
