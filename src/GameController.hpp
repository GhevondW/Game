
#ifndef GameController_hpp
#define GameController_hpp

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Models/Element.h"
#include "DataProviders/ConfigDataProvider.h"

#endif /* GameController_hpp */

using namespace sf;

enum GameStatus {
    NotStarted,
    Started,
    Won,
    Failed
};

class GameController {

public :
    GameController();

    void StartGame();
    void UpdateGameStatus(GameStatus &status);

private:
    void _run();

private:
    GameStatus _gameStatus = NotStarted;
    RenderWindow *_app;
    std::unique_ptr<game::ElementFactory>           _factory{nullptr};
    std::unique_ptr<game::JSONConfigDataProvider>   _config_dp{nullptr};
};