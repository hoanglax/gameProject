#ifndef GAME_H
#define GAME_H

#include "CommonFunc.h" 
#include "map.h"
#include "object.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "collision.h"

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
    void checkCollisions();
    bool CheckCollision(const SDL_Rect& a ,const SDL_Rect& b);

    //threats
    bool loadThreats();
    void updateThreats();
    void renderThreats();
    vector<ThreatObject*> MakeThreatList();
    
private:
    bool isRunning;
    GameMap gameMap;
    MainObject player;
    ImpTimer fps_timer;
    //threats
    vector<ThreatObject*> threats;
};

#endif // GAME_H
