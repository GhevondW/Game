#include <SFML/Graphics.hpp>
#include "GameController.hpp"

using namespace sf;

GameController::GameController() {
}

void GameController::updateGameStatus(GameStatus &status) {
    _gameStatus = status;
}

void GameController::startGame() {
    _app = new RenderWindow(VideoMode(744, 900), "Game", Style::Close);
    _app->setFramerateLimit(60);
    run();
}

void GameController::run() {
    Texture texture;
    texture.loadFromFile("C:\\Users\\Ghevond\\Desktop\\Game\\DevTestGame\\resources\\bomb.png");
    Sprite sprite(texture);
    Sprite sprite2(texture);
    sprite.setPosition(100, 100);
    sprite2.setPosition(200, 100);

    auto ret = texture.getSize();

    while (_app->isOpen()) {
        _app->clear(Color(150, 150, 150, 255));
        _app->draw(sprite);
        _app->draw(sprite2);
        sf::Event event;
        while (_app->pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                _app->close();
        }
        _app->display();
    }
}
