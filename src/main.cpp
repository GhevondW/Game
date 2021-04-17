#include <iostream>
#include "GameController.hpp"
#include "DataProviders/ConfigDataProvider.h"

int main() {
    GameController* gc = new GameController();

    gc->startGame();
    return 0;
}
