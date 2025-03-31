#ifndef GAME_H
#define GAME_H

#include "CommonFunc.h" 
#include "map.h"
#include "object.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "BoostObject.h"
#include "menu.h"
#include "Music.h"

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

    //Item
    bool loadLifeItem();
    void updateLifeItem();
    void renderLifeItem();
    vector<LifeItem*> MakeLifeItemList();

    //player
    void renderLife();

    //handle menu
    int handleGamePause();
    int handleGameOver();
    int handlePlayerWon();

private:
    bool isRunning;
    bool isInvincible = false;
    bool isPause = false;
    GameMap gameMap;
    MainObject player;
    ImpTimer fps_timer;
    ImpTimer invincibleTimer;
    Music gMusic;
    Music lost_music;
    Music won_music;
    //threats
    vector<ThreatObject*> threats;
    //player
    int life = PLAYER_LIFE;
    SDL_Texture* heart_texture;
    //item
    vector<LifeItem*> lifeItems;

    ImpTimer gameTimer;
    TTF_Font* timerFont;
    SDL_Color textColor = { 255, 255, 255 }; 
};

#endif // GAME_H
