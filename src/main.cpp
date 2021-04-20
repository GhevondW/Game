#include <iostream>
#include "GameController.hpp"
#include "DataProviders/ConfigDataProvider.h"
#include "DataProviders/ResourceDataProvider.h"
#include "Models/Element.h"

int main() {
    GameController* gc = new GameController();
    gc->StartGame();
    return 0;
}
