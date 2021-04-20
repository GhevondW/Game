
#ifndef GameController_hpp
#define GameController_hpp

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "Models/Element.h"
#include "DataProviders/ConfigDataProvider.h"
#include "Board.h"

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
    void UpdateGameStatus(GameStatus status);

private:

    void _LoadInitialData();
    void _Run();

private:
    GameStatus _gameStatus = NotStarted;
    RenderWindow* _app{nullptr};

    std::shared_ptr<game::IConfigDataProvider>      _config_dp{ nullptr };
    std::shared_ptr<game::IResourceDataProvider>    _resource_dp{ nullptr };
    std::unique_ptr<game::IBoard>                   _board{ nullptr };
    std::shared_ptr<game::KernelProvider>           _kernels{ nullptr };
};
