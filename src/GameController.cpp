#include <SFML/Graphics.hpp>
#include "GameController.hpp"

using namespace sf;

GameController::GameController() {
}

void GameController::updateGameStatus(GameStatus &status) {
    _gameStatus = status;
}

void GameController::startGame() {
    _app = new RenderWindow(VideoMode(744, 1080), "Game", Style::Close);
    _app->setFramerateLimit(60);
    run();
}

void GameController::run() {
    Texture texture;
    texture.loadFromFile("PATH_TO_RESOURCES/Resources/bomb.png");
    Sprite sprite(texture);
    sprite.setPosition(100, 100);

    while (_app->isOpen()) {
        _app->clear(Color(150, 150, 150, 255));
        _app->draw(sprite);
        sf::Event event;
        while (_app->pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                _app->close();
        }
        _app->display();
    }
}
