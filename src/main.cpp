#include <iostream>
#include "GameController.hpp"
#include "DataProviders/ConfigDataProvider.h"
#include "DataProviders/ResourceDataProvider.h"
#include "Models/Element.h"

int main() {
    GameController* gc = new GameController();
    gc->StartGame();

    //game::IConfigDataProvider* dp = new game::JSONConfigDataProvider();
    //const auto& ref = dp->Load("C:\\Users\\Ghevond\\Desktop\\Game\\DevTestGame\\resources\\config.json");

    //game::IResourceDataProvider* rp = new game::JSONResourceDataProvider();
    //const auto& ref2 = rp->Load("C:\\Users\\Ghevond\\Desktop\\Game\\DevTestGame\\resources\\res.json");

    //delete dp;
    //delete rp;
    return 0;
}
