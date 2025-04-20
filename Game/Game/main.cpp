#include "CommonFunc.h"
#include "CommonLib.h"
#include "game.h"
#include "menu.h"
#include "Music.h"


int main(int argc, char* argv[])
{

    Game game;

    if (!game.init()) return -1;
    if (!game.loadResources()) return -1;
    game.run();

    game.clean();
    return 0;
}
