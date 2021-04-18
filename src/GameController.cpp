#include <SFML/Graphics.hpp>
#include "GameController.hpp"
#include "Models/Element.h"

using namespace sf;
using namespace game;

GameController::GameController() {
    _config_dp = std::make_shared<JSONConfigDataProvider>();
    _resource_dp = std::make_shared<JSONResourceDataProvider>();
    _board = std::make_unique<Board>(_config_dp, _resource_dp);
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
    //Texture texture;
    //texture.loadFromFile("C:\\Users\\Ghevond\\Desktop\\Game\\DevTestGame\\resources\\bomb.png");
    //Texture texture_tile;
    //texture_tile.loadFromFile("C:\\Users\\Ghevond\\Desktop\\Game\\DevTestGame\\resources\\tile_1.png");
    //
    //{
    //    Sprite sprite(texture);
    //    Sprite sprite2(texture_tile);
    //    sprite2.setPosition(100, 100);
    //    sprite.setPosition(100, 200);
    //}

    //Sprite sprite(texture);
    //Sprite sprite2(texture_tile);
    //sprite2.setPosition(10, 200);
    //sprite.setPosition(10, 100);
    //sprite.setScale(Vector2f{ 1, 1 });
    //sprite2.setScale(Vector2f{ 1, 1 });

    //sprite.setTextureRect(IntRect{0,0,50,50});
    //sprite2.setTextureRect(IntRect{ 0,0,50,50 });

    //std::vector<Element*> elements;
    //elements.push_back(_factory->CreateElement(Element::RED));
    //elements.push_back(_factory->CreateElement(Element::GREEN));
    //elements.push_back(_factory->CreateElement(Element::RED));
    //elements.push_back(_factory->CreateElement(Element::VIOLET));
    //elements.push_back(_factory->CreateElement(Element::VBOMB));
    //elements.push_back(_factory->CreateElement(Element::VIOLET));
    //elements.push_back(_factory->CreateElement(Element::VIOLET));
    //elements.push_back(_factory->CreateElement(Element::RED));
    //elements.push_back(_factory->CreateElement(Element::GREEN));
    //elements.push_back(_factory->CreateElement(Element::RED));
    //elements.push_back(_factory->CreateElement(Element::VIOLET));
    //elements.push_back(_factory->CreateElement(Element::VBOMB));
    //elements.push_back(_factory->CreateElement(Element::VIOLET));
    //elements.push_back(_factory->CreateElement(Element::VIOLET));

    while (_app->isOpen()) {
        _app->clear(Color(204, 255, 204, 255));

        //for (size_t i = 0; i < elements.size(); i++)
        //{
        //    elements[i]->SetPosition(i * 70, 200);
        //    _app->draw(elements[i]->GetSprite());
        //}
        
        _board->Draw(_app);

        sf::Event event;
        while (_app->pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                _app->close();
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
