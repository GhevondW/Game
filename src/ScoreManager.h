#ifndef _SCORE_MANAGER_H_
#define _SCORE_MANAGER_H_

#include "DataProviders/ConfigDataProvider.h"
#include "Models/Element.h"
#include "IDrawable.h"

namespace game
{

	

	struct IScoreController
	{

		enum STATUS
		{
			Won,
			Lose,
			Game
		};

		IScoreController() = default;
		virtual ~IScoreController() = default;

		virtual auto GetObjectivesMap() const -> const std::map<Element::TYPE, std::pair<int, Element*>> & = 0;
		virtual auto UpdateScore(Element::TYPE, int count) -> void = 0;
		virtual auto UpdateMovesCount() -> bool = 0;
		virtual auto CheckGameStatus() const ->STATUS = 0;
	};

	struct IScoreManager : public IScoreController, public IDrawable
	{
		IScoreManager() = default;
		virtual ~IScoreManager() = default;
	};

	class ScoreManager : public IScoreManager
	{
	public:
		ScoreManager(IObjectiveProvider* objectives,
					IElementFactory* factory);
		~ScoreManager();

		auto Draw(sf::RenderWindow& window) -> void;
		auto GetObjectivesMap() const -> const std::map<Element::TYPE, std::pair<int, Element*>>& { return _objectives; }
        auto UpdateScore(Element::TYPE, int count) -> void;
        auto UpdateMovesCount() -> bool;
		auto CheckGameStatus() const->STATUS;

    private:
        
        auto _CreateText(const std::string& text, size_t) const -> sf::Text;
		auto _Init() -> bool;
        
	private:
		IObjectiveProvider*									_objectives_provider_ref{nullptr};
		IElementFactory*									_factory_ref{nullptr};
		std::map<Element::TYPE, std::pair<int, Element*>>	_objectives{};
		sf::Font											_font{};
        
        int                                                 _total_count{};
        int                                                 _moves_count{};
	};
}

#endif // !_SCORE_MANAGER_H_
