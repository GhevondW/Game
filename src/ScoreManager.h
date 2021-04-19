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

		bool Init();
		void Draw(sf::RenderWindow& window);
		const std::map<Element::TYPE, std::pair<int, Element*>>& GetObjectivesMap() const { return _objectives; }

	private:
		IObjectiveProvider*									_objectives_provider_ref{nullptr};
		ElementFactory*										_factory_ref{nullptr};
		std::map<Element::TYPE, std::pair<int, Element*>>	_objectives{};
		sf::Font											_font{};
	};
}

#endif // !_SCORE_MANAGER_H_
