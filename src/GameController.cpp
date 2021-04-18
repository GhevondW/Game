#include <SFML/Graphics.hpp>
#include "GameController.hpp"
#include "Models/Element.h"

using namespace sf;
using namespace game;

#define X 110
#define Y 230

GameController::GameController() {
    _config_dp = std::make_shared<JSONConfigDataProvider>();
    _resource_dp = std::make_shared<JSONResourceDataProvider>();
    _board = std::make_unique<Board>(_config_dp, _resource_dp, X, Y);
}

void GameController::UpdateGameStatus(GameStatus &status) {
    _gameStatus = status;
}

void GameController::StartGame() {
    _LoadInitialData();
    _board->Init();
    _app = new RenderWindow(VideoMode(750, 950), "Game", Style::Close);
    _app->setFramerateLimit(60);
    _Run();
}

void GameController::_Run() {

    while (_app->isOpen()) {
        _app->clear(Color(204, 255, 204, 255));

        _board->Draw(*_app);

        sf::Event event;
        while (_app->pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                _app->close();
            else
                _board->HandleClick(event);
        }
        _app->display();
    }

}

void GameController::_LoadInitialData()
{
    std::string rpath = R_PATH;
    _config_dp->Load(rpath + "config.json");
    _resource_dp->Load(rpath + "res.json");
}
