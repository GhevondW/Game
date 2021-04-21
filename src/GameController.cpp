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
    _kernels = std::make_shared<KernelProvider>();
    _board = std::unique_ptr<Board>(nullptr);
}

void GameController::UpdateGameStatus(GameStatus status) {
    _gameStatus = status;
}

void GameController::StartGame() {
    _LoadInitialData();

    _board = std::make_unique<Board>(_config_dp, _resource_dp, _kernels, X, Y);
    
    if(!_board->Init())
        UpdateGameStatus(GameStatus::Failed);
    
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


    _kernels->AddKernel({{1,1},
                        {1,1}}, Element::TYPE::BOMB);

    _kernels->AddKernel({{1,1,1}});

    _kernels->AddKernel({{1},
                         {1},
                         {1}});

    _kernels->AddKernel({{1,1,1,1}}, Element::TYPE::VBOMB);

    _kernels->AddKernel({{1},
                         {1},
                         {1},
                         {1}}, Element::TYPE::HBOMB);
}
