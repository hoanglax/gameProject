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
#include "SoundEffect.h"

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
    void renderLifeItem();
    vector<LifeItem*> MakeLifeItemList();
    bool loadScoreItem();
    void renderScoreItem();
    vector<ScoreItem*> MakeScoreItemList();

    //player
    void renderLife();

    //handle menu
    int handleStartMenu();
    int handleGamePause();
    int handleGameOver();
    int handlePlayerWon();

    //high score
    void loadHighScore(const string& filename);
    void saveHighScore(const string& filename);
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
    Music start_music;
    SoundEffect move_sound;
    SoundEffect hit_sound;
    SoundEffect buff_sound;
    SoundEffect click_sound;
    //threats
    vector<ThreatObject*> threats;
    //player
    int life = PLAYER_LIFE;
    int bonusScore = 0;
    SDL_Texture* heart_texture;
    //item
    vector<LifeItem*> lifeItems;
    vector<ScoreItem*> scoreItems;

    ImpTimer gameTimer;
    TTF_Font* timerFont;
    SDL_Color textColor = { 255, 255, 255 };

    StartMenu* startMenu;

    int score;
    int highScore;
};

#endif // GAME_H
