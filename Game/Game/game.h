#ifndef GAME_H
#define GAME_H

#include "CommonFunc.h" 
#include "map.h"
#include "object.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "menu.h"

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
    void restartGame();
    void clean();
    void checkCollisions();
    bool CheckCollision(const SDL_Rect& a ,const SDL_Rect& b);

    //threats
    bool loadThreats();
    void updateThreats();
    void renderThreats();
    vector<ThreatObject*> MakeThreatList();
    //player
    void renderLife();

    int handleGameOver();
    int handlePlayerWon();

private:
    bool isRunning;
    bool isInvincible = false;
    GameMap gameMap;
    MainObject player;
    ImpTimer fps_timer;
    ImpTimer invincibleTimer;
    ImpTimer g_time;
    //threats
    vector<ThreatObject*> threats;
    //player
    int life = PLAYER_LIFE;
    SDL_Texture* heart_texture;
};

#endif // GAME_H
