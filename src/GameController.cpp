#include <SFML/Graphics.hpp>
#include "GameController.hpp"
#include "Models/Element.h"
#include "Message.h"

using namespace sf;
using namespace game;

#define X 110
#define Y 230

GameController::GameController() {
    _config_dp      = std::make_shared<JSONConfigDataProvider>();
    _resource_dp    = std::make_shared<JSONResourceDataProvider>();
    _kernels        = std::make_shared<KernelProvider>();
    if(_LoadInitialData()) {
        _factory        = std::make_shared<ElementFactory>(_resource_dp);
        _score_manager  = std::make_shared<ScoreManager>(_config_dp.get(), _factory.get());
        _board          = std::unique_ptr<Board>(nullptr);
    }
}

void GameController::UpdateGameStatus(GameStatus status) {
    _gameStatus = status;
}

void GameController::StartGame() {
    UpdateGameStatus(GameStatus::Started);

    if (_config_dp == nullptr || _resource_dp == nullptr || _kernels == nullptr || _factory == nullptr || _score_manager == nullptr) 
    {
        UpdateGameStatus(GameStatus::LoadInitialDataFailed);
    }
    
    _app = new RenderWindow(VideoMode(750, 950), "Game", Style::Close);
    _app->setFramerateLimit(60);

    _board = std::make_unique<Board>(_config_dp,
        _factory,
        _score_manager,
        _kernels,
        _app->getSize().x,
        _app->getSize().y);

    if (!_board->Init())
        UpdateGameStatus(GameStatus::Failed);

    _Run();
}

void GameController::_Run() {

    while (_app->isOpen()) {
        _app->clear(Color(204, 255, 204, 255));

        _status = _score_manager->CheckGameStatus();

        _Draw();

        sf::Event event;
        while (_app->pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                _app->close();
            }
            else {
                if (_status == IScoreController::STATUS::Game) {
                    _board->HandleClick(event);
                }
            }
        }
        _app->display();
    }
}

void GameController::_Draw()
{
    if (_status == IScoreController::STATUS::Won) {
        AlertMessage msg{ "You Win!" };
        msg.Draw(*_app);
    }
    else if (_status == IScoreController::STATUS::Lose) {
        AlertMessage msg{ "You Lose!" };
        msg.Draw(*_app);
    }
    else {
        _board->Draw(*_app);
        _score_manager->Draw(*_app);
    }
}

bool GameController::_LoadInitialData()
{
    std::string rpath = R_PATH;
    if (_config_dp->Load(rpath + "config.json") != LoadStatus::OK) return false;
    if (_resource_dp->Load(rpath + "res.json") != LoadStatus::OK) return false;


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

    return true;
}
