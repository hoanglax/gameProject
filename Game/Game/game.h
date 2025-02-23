#ifndef GAME_H
#define GAME_H

#include "CommonFunc.h" 
#include "map.h"
#include "object.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"

class Game
{
public:
    Game();
    ~Game();

    bool init();
    bool loadResources();
    void run();
    void handleEvents();
    void update();
    void render();
    void clean();

    //threats
    bool loadThreat();
    void updateThreat();
    void renderThreat();

private:
    bool isRunning;
    GameMap gameMap;
    MainObject player;
    ImpTimer fps_timer;
    ThreatObject threat;
};

#endif // GAME_H
