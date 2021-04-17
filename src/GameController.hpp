
#ifndef GameController_hpp
#define GameController_hpp

#include <SFML/Graphics.hpp>
#include <stdio.h>

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

    void startGame();

    void updateGameStatus(GameStatus &status);

private:
    GameStatus _gameStatus = NotStarted;
    RenderWindow *_app;

    void run();


};