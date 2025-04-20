#include "game.h"

bool initData()
{
    bool success = true;
    bool ret = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0 || SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        cerr << "Error: SDL Init - " << SDL_GetError() << "\n";
        cerr << "Error: TTF Init - " << TTF_GetError() << "\n";
        cerr << "Error: Mixer Init - " << Mix_GetError() << "\n";
        ret = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! ERROR: %s", Mix_GetError());
        ret = false;
    }

    if (ret == false)
        return false;
    else 
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
                    g_window = SDL_CreateWindow("Mell bell don't like vegatable",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH, SCREEN_HEIGHT,
                    SDL_WINDOW_FULLSCREEN);
    }
    if (g_window == NULL)
        success = false;
    else 
    {
            g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
    }

    if (g_screen == NULL)
    {
        cerr << "Renderer (g_screen) is NULL!" << endl;
        return false;
    }
    return success;
}


Game::Game()
{   
    isRunning = true;
    startMenu = nullptr;
    heart_texture = nullptr;
    timerFont = nullptr;
    score = 0;
    highScore = 0;
}

Game::~Game()
{
    clean();
}

bool Game::init()
{
    srand(time(0));
    if (!initData())
        return false;

    if (TTF_Init() < 0) {
        cerr << "TTF_Init failed: " << TTF_GetError() << endl;
        clean();
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Mix_OpenAudio failed: " << Mix_GetError() << endl;
        clean();
        return false;
    }

    startMenu = new StartMenu(g_screen);
    if (!startMenu->init("data/Menu.png"))
    {
        cerr << "Fail to load start menu" << endl;
        clean();
        return false;
    }
    if (!start_music.loadMusic("music/game_theme.mp3"))
    {
        cerr << "Fail to load start music";
        clean();
        return false;
    }

    return true;
}

bool Game::loadResources()
{
    gameMap.loadMap("map/map02.dat");
    gameMap.loadTiles(g_screen);

    if (!player.loadImg("player/player_idle.png", g_screen))
        return false;
    player.set_clips();

    if (!loadThreats())
        return false;

    if (!loadLifeItem() || !loadScoreItem())
        return false;

    heart_texture = IMG_LoadTexture(g_screen, "player/heart.png");
    if (!heart_texture)
    {
        cerr << "Fail to load heart image!";
        return false;
    }

    timerFont = TTF_OpenFont("data/font.ttf", 24); 
    if (!timerFont) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    
    if (!click_sound.loadSound("sfx/click.wav"))
    {
        return false;
    }
    return true;
}

void Game::run()
{
    while (true)
    {
        start_music.play(-1);
        int menuAction = handleStartMenu();
        start_music.stop();

        if (menuAction == EXIT)
        {   
            isRunning = false;
            break;
        }
        else if (menuAction != START)
        {
            continue;
        }

        if (!gMusic.loadMusic("music/play_theme.mp3") ||
            !lost_music.loadMusic("music/lost_theme.mp3") ||
            !won_music.loadMusic("music/won_theme.mp3"))
        {
            break; 
        }

        if (!hit_sound.loadSound("sfx/hit.wav") ||
            !buff_sound.loadSound("sfx/buff.wav") ||
            !move_sound.loadSound("sfx/move.wav"))
        {
            break;
        }

        gMusic.play(-1);

        gameTimer.start();
        isRunning = true;

        while (isRunning)
        {
            fps_timer.start();
            handleEvents();
            update();
            render();

            int real_imp_time = fps_timer.get_ticks();
            int time_per_frame = 1000 / FPS;

            if (real_imp_time < time_per_frame)
            {
                int delay_time = time_per_frame - real_imp_time;
                if (delay_time >= 0)
                    SDL_Delay(delay_time);
            }

            if (life == 0)
            {
                gMusic.pause();
                lost_music.play(-1);
                int action = handleGameOver();
                lost_music.stop();

                if (action == EXIT)
                {
                    isRunning = false;
                    restartGame();
                    lost_music.freeMusic();
                }
                else if (action == RESTART)
                {
                    restartGame(); 
                    gMusic.play(-1);
                }
            }

            if (player.is_won())
            {
                gMusic.pause();
                won_music.play(-1);
                int action = handlePlayerWon();
                won_music.stop();

                if (action == EXIT)
                {
                    isRunning = false;
                    restartGame();
                    won_music.freeMusic();
                }
                else if (action == RESTART)
                {
                    restartGame(); 
                    gMusic.play(-1);
                }
            }

            if (isPause)
            {
                int action = handleGamePause();
                if (action == EXIT)
                {
                    isRunning = false;
                    restartGame();
                }
                else if (action == RESUME)
                {
                    isPause = false;
                }
            }
        }

        gMusic.freeMusic();
        lost_music.freeMusic();
        won_music.freeMusic();
        move_sound.freeSound();
        hit_sound.freeSound();
        buff_sound.freeSound();
    }
}

void Game::handleEvents()
{
    while (SDL_PollEvent(&g_event) != 0)
    {
        if (g_event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        if (g_event.type == SDL_KEYDOWN)
        {
            if (g_event.key.keysym.sym == SDLK_ESCAPE)
            {
                isPause = !isPause;
            }
        }
        player.HandelInputAction(g_event, g_screen, move_sound);
    }
}

void Game::update()
{
    Map map_data = gameMap.get_map();
    player.SetMapXY(map_data.start_x, map_data.start_y);
    player.DoPlayer(map_data);
    gameMap.SetMap(map_data);
    updateThreats();
    checkCollisions();
}

//threats
bool Game::loadThreats()
{
    threats = MakeThreatList(); // Lấy danh sách threats từ hàm MakeThreatList
    return !threats.empty(); // Kiểm tra xem có tải thành công không
}
void Game::renderThreats()
{
    float cam_y = player.get_y_pos() - SCREEN_HEIGHT / 2;
    for (auto threat : threats) {
        threat->Show(g_screen, cam_y);
    }
}
void Game::updateThreats()
{
    Map map_data = gameMap.get_map();
    for (auto threat : threats) {
        threat->DoPlayer(map_data);
    }
}
vector<ThreatObject*> Game::MakeThreatList()
{
    vector<ThreatObject*> list_threats;
    vector<string> threatFiles_left = { "threats/car01_left.png", "threats/car02_left.png", "threats/car03_left.png" };
    vector<string> threatFiles_right = { "threats/car01_right.png", "threats/car02_right.png", "threats/car03_right.png" };

    for (int i = 0; i < LEFT_THREAT_NUMBER; i++)
    {
        //  Threat đi từ TRÁI -> PHẢI
        ThreatObject* p_threat_left = new ThreatObject();
        if (p_threat_left)
        {
            int randomIndex = rand() % 3;
            if (!p_threat_left->loadImg(threatFiles_left[randomIndex], g_screen))
            {
                cerr << "Failed to load threat image!\n";
                delete p_threat_left;
                continue;
            }

            p_threat_left->set_clips();
            p_threat_left->set_x_pos(LEFT);
            p_threat_left->set_y_pos(8 * TILE_SIZE + i * 4 * TILE_SIZE);
            p_threat_left->set_start_x(LEFT);
            p_threat_left->set_start_y(8 * TILE_SIZE + i * 4 * TILE_SIZE);
            p_threat_left->set_direction(1);

            float threat_speed = THREAT_SPEED + rand() % 50;
            p_threat_left->set_speed(threat_speed);

            list_threats.push_back(p_threat_left);
        }

        //  Threat đi từ PHẢI -> TRÁI
        ThreatObject* p_threat_right = new ThreatObject();
        if (p_threat_right)
        {
            int randomIndex = rand() % 3;
            if (!p_threat_right->loadImg(threatFiles_right[randomIndex], g_screen))
            {
                cerr << "Failed to load threat image!\n";
                delete p_threat_right;
                continue;
            }

            p_threat_right->set_clips();
            p_threat_right->set_x_pos(RIGHT);
            p_threat_right->set_y_pos(7 * TILE_SIZE + i * 4 * TILE_SIZE);
            p_threat_right->set_start_x(RIGHT);
            p_threat_right->set_start_y(7 * TILE_SIZE + i * 4 * TILE_SIZE);
            p_threat_right->set_direction(-1);

            float threat_speed = THREAT_SPEED + rand() % 50; 
            p_threat_right->set_speed(threat_speed);

            list_threats.push_back(p_threat_right); 
        }
    }

    return list_threats;
}

//Item
bool Game::loadLifeItem()
{
    lifeItems = MakeLifeItemList();
    return !lifeItems.empty();
}
void Game::renderLifeItem()
{
    float cam_y = player.get_y_pos() - SCREEN_HEIGHT / 2;
    for (auto lifeItem : lifeItems) {
        lifeItem->Show(g_screen, cam_y);
    }
}
vector<LifeItem*> Game::MakeLifeItemList()
{
    vector<LifeItem*> list_life_item;
    string mcdonald = "data/mcdonald.png";
  
    for (int i = 0; i < 10; i++)
    {
        int min_row = 6;
        int max_row = max_map_y - 10;

        int tile_x = rand() % max_map_x;
        int tile_y = min_row + rand() % (max_row - min_row);

        LifeItem* life_item = new LifeItem();
        life_item->set_x_pos(tile_x * TILE_SIZE); 
        life_item->set_y_pos(tile_y * TILE_SIZE);

        if (!life_item->loadImg(mcdonald, g_screen)) {
            cerr << "Failed to load life item image: " << mcdonald << endl;
            delete life_item;
            continue;
        }

        list_life_item.push_back(life_item);
    }
    
    return list_life_item;
}

bool Game::loadScoreItem()
{
    scoreItems = MakeScoreItemList();
    return !scoreItems.empty();
}
void Game::renderScoreItem()
{
    float cam_y = player.get_y_pos() - SCREEN_HEIGHT / 2;
    for (auto scoreItem : scoreItems) {
        scoreItem->Show(g_screen, cam_y);
    }
}
vector<ScoreItem*> Game::MakeScoreItemList()
{
    vector<ScoreItem*> list_score_item;
    string drumstick = "data/drumstick.png";

    for (int i = 0; i < 10; i++)
    {
        int min_row = 6;
        int max_row = max_map_y - 10;

        int tile_x = rand() % max_map_x;
        int tile_y = min_row + rand() % (max_row - min_row);

        ScoreItem* score_item = new ScoreItem();
        score_item->set_x_pos(tile_x * TILE_SIZE);
        score_item->set_y_pos(tile_y * TILE_SIZE);

        if (!score_item->loadImg(drumstick, g_screen)) {
            cerr << "Failed to load life item image: " << drumstick << endl;
            delete score_item;
            continue;
        }

        list_score_item.push_back(score_item);
    }

    cerr << "Total life items created: " << list_score_item.size() << endl;
    return list_score_item;
}
//render
void Game::render()
{
    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    SDL_RenderClear(g_screen);

    gameMap.DrawMap(g_screen);
    player.Show(g_screen);
    renderLife();
    renderLifeItem();
    renderScoreItem();
    renderThreats(); 


    Uint32 elapsedTime = gameTimer.get_ticks() / 1000;
    string timeText = "Time: " + to_string(elapsedTime) + "s" + "   |    Bonus score: " +to_string(bonusScore);

    SDL_Surface* textSurface = TTF_RenderText_Solid(timerFont, timeText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);

    SDL_Rect renderQuad = { 10, 10, textSurface->w, textSurface->h }; 
    SDL_RenderCopy(g_screen, textTexture, NULL, &renderQuad);

    if (!textSurface) {
        cerr << "Failed to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
        return;
    }


    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(g_screen);
}
void Game::renderLife()
{
    int start_X = SCREEN_WIDTH - 40;
    int start_Y = 10;

    for (int i = 0; i < life; i++)
    {
        SDL_Rect heartRect = { start_X - i * 35 , start_Y , 30 , 30 };
        SDL_RenderCopy(g_screen, heart_texture, NULL, &heartRect);
    }
}

//check Collisions
void Game::checkCollisions()
{
    SDL_Rect playerRect = player.getHitboxRect();
    for (auto threat : threats)
    {
        SDL_Rect threatRect = threat->GetRectFrame();

        if (CheckCollision(playerRect, threatRect) && !isInvincible)
        {
            life--;
            hit_sound.play();
            player.take_damage();
            isInvincible = true;
            invincibleTimer.start();
        }

        if (isInvincible && invincibleTimer.get_ticks() >= 1000)
        {
            isInvincible = false;
            invincibleTimer.stop();
        }
    }
    for (auto it = lifeItems.begin(); it != lifeItems.end();)
    {
        LifeItem* item = *it;
        SDL_Rect itemRect = item->GetRectFrame();
        
        itemRect.x += 16;    
        itemRect.w += 16;
        itemRect.h -= 8;
        itemRect.y += 8;

        if (CheckCollision(playerRect, itemRect))
        {
            player.picked_item();
            life++;
            buff_sound.play();
            it = lifeItems.erase(it);  
            delete item;
        }
        else
        {
            ++it;
        }
    }
    for (auto it = scoreItems.begin(); it != scoreItems.end();)
    {
        ScoreItem* item = *it;
        SDL_Rect itemRect = item->GetRectFrame();

        itemRect.x += 16;
        itemRect.w += 16;
        itemRect.h -= 8;
        itemRect.y += 8;

        if (CheckCollision(playerRect, itemRect))
        {   
            player.picked_item();
            bonusScore += 100;
            buff_sound.play();
            it = scoreItems.erase(it);
            delete item;
        }
        else
        {
            it++;
        }
    }
}
bool Game::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // Case 1: size object 1 < size object 2
    if (left_a > left_b && left_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    // Case 2: size object 1 < size object 2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    // Case 3: size object 1 = size object 2
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
        return true;
    }

    return false;
}
//game over
int Game::handleStartMenu()
{
    start_music.play(-1);  
    bool isStart = true;
    int action = NONE;

    while (isStart)
    {
        action = startMenu->handleEvent(click_sound);

        SDL_RenderClear(g_screen);
        startMenu->render();
        SDL_RenderPresent(g_screen);

        if (action == START || action == EXIT)
        {
            isStart = false;
        }
        else if (action == HOW_TO_PLAY)
        {
            HowToPlayScreen how_to_play(g_screen);
            if (!how_to_play.init("data/how_to_play.png"))
            {
                cerr << "Error: Init how to play screen" << endl;
                continue;
            }

            int subAction = NONE;
            while (true)
            {
                subAction = how_to_play.handleEvent(click_sound);

                SDL_RenderClear(g_screen);
                how_to_play.render();
                SDL_RenderPresent(g_screen);

                if (subAction == BACK_TO_MENU)
                    break;
                else if (subAction == EXIT)
                {
                    start_music.stop();
                    start_music.freeMusic();
                    isRunning = false;
                    return EXIT;
                }
            }
        }
    }

    start_music.stop();
    return action;
}
int Game::handleGamePause()
{
    PauseScreen gamePause(g_screen);
    if (!gamePause.init("data/Game_paused.png"))
    {
        cerr << "Error: Fail to load pause screen\n";
        return EXIT;  
    }

    bool gamePauseRuning = true;
    int action = NONE;

    while (gamePauseRuning)
    {
        action = gamePause.handleEvent(click_sound);

        SDL_RenderClear(g_screen);
        gamePause.render();
        SDL_RenderPresent(g_screen);

        if (action == RESUME || action == EXIT)
        {
            gamePauseRuning = false;
        }
    }

    std::cerr << "Exiting Pause Screen with action: " << action << "\n";
    SDL_RenderClear(g_screen);
    return action;
}
int Game::handleGameOver()
{
    GameOverScreen gameOver(g_screen);

    Uint32 elapsedTime = gameTimer.get_ticks() / 1000;
   
    int currentScore = bonusScore;

    int highScore = 0;
    ifstream fileIn("data/highscore.txt");
    if (fileIn.is_open())
    {
        fileIn >> highScore;
        fileIn.close();
    }

    if (currentScore > highScore)
    {
        highScore = currentScore;
        std::ofstream fileOut("data/highscore.txt");
        fileOut << highScore;
        fileOut.close();
    }

    if (!gameOver.init("data/GameOver.png"))
    {
        cerr << "Error: Fail to load game over screen\n";
        return EXIT;
    }

    TTF_Font* font = TTF_OpenFont("data/font.ttf", 40);
    if (!font) {
        cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        return EXIT;
    }

    string text = "Time: " + to_string(elapsedTime) + "s | Score: " + to_string(currentScore);
    string highscoreText = "High score: " + to_string(highScore);
    SDL_Color textColor = { 0, 0, 0 };


    bool gameOverRunning = true;
    int action = NONE;

    while (gameOverRunning)
    {
        action = gameOver.handleEvent(click_sound);

        SDL_RenderClear(g_screen);
        gameOver.render();

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);

        int textW = textSurface->w;
        int textH = textSurface->h;
        SDL_Rect renderQuad = { (SCREEN_WIDTH - textW) / 2, 256, textW, textH };

        SDL_RenderCopy(g_screen, textTexture, NULL, &renderQuad);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);



        SDL_Surface* highSurface = TTF_RenderText_Solid(font, highscoreText.c_str(), textColor);
        SDL_Texture* highTexture = SDL_CreateTextureFromSurface(g_screen, highSurface);

        int highW = highSurface->w;
        int highH = highSurface->h;
        SDL_Rect highRect = { (SCREEN_WIDTH - highW) / 2, 256 + textH + 16, highW, highH };
        SDL_RenderCopy(g_screen, highTexture, NULL, &highRect);
        SDL_FreeSurface(highSurface);
        SDL_DestroyTexture(highTexture);


        SDL_RenderPresent(g_screen);

        if (action == RESTART || action == EXIT)
        {
            cerr << "Game Over action: " << action << endl;
            gameOverRunning = false;
        }
    }

    return action;
}
int Game::handlePlayerWon()
{
    WonScreen won_screen(g_screen);
    Uint32 elapsedTime = gameTimer.get_ticks() / 1000;
    int score = 1000 - (elapsedTime * 10);
    if (score < 0) score = 0;
    score += bonusScore;

    int highScore = 0;
    std::ifstream infile("data/highscore.txt");
    if (infile.is_open())
    {
        infile >> highScore;
        infile.close();
    }

    if (score > highScore)
    {
        highScore = score;
        std::ofstream outfile("data/highscore.txt");
        outfile << highScore;
        outfile.close();
    }

    if (!won_screen.init("data/won.png"))
    {
        cerr << "Error: Fail to load won screen\n";
        return EXIT;
    }

    TTF_Font* font = TTF_OpenFont("data/font.ttf", 40);
    if (!font) {
        cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        return EXIT;
    }

    string text = "Time: " + to_string(elapsedTime) + "s | Total Score: " + to_string(score);
    string highscoreText = "High score: " + to_string(highScore);
    SDL_Color textColor = { 0, 0, 0 };

    bool is_playe_won = true;
    int action = NONE;

    while (is_playe_won)
    {
        action = won_screen.handleEvent(click_sound);

        SDL_RenderClear(g_screen);
        won_screen.render();

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);

        int textW = textSurface->w;
        int textH = textSurface->h;
        SDL_Rect renderQuad = { (SCREEN_WIDTH - textW) / 2, 440, textW, textH };

        SDL_RenderCopy(g_screen, textTexture, NULL, &renderQuad);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);



        SDL_Surface* highSurface = TTF_RenderText_Solid(font, highscoreText.c_str(), textColor);
        SDL_Texture* highTexture = SDL_CreateTextureFromSurface(g_screen, highSurface);

        int highW = highSurface->w;
        int highH = highSurface->h;
        SDL_Rect highRect = { (SCREEN_WIDTH - highW) / 2, 440 + textH + 16, highW, highH };
        SDL_RenderCopy(g_screen, highTexture, NULL, &highRect);
        SDL_FreeSurface(highSurface);
        SDL_DestroyTexture(highTexture);

        SDL_RenderPresent(g_screen);

        if (action == RESTART || action == EXIT)
        {
            is_playe_won = false;
        }
    }

    TTF_CloseFont(font);
    return action;
}

void Game::restartGame()
{
    life = PLAYER_LIFE;
    bonusScore = 0;
    player.resetPosition();
    gMusic.play(-1);
    isPause = false;
    for (auto* threat : threats) 
    {
        delete threat;
    }
    threats.clear();
    for (auto* item : lifeItems)
    {
        delete item;
    }
    lifeItems.clear();
    gameTimer.start();
    threats = MakeThreatList();
    lifeItems = MakeLifeItemList();
}
void Game::loadHighScore(const string& filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }

    else
    {
        highScore = 0;
    }
}
void Game::saveHighScore(const string& filename)
{
    ofstream file(filename);
    if (file.is_open())
    {
        file << highScore;
        file.close();
    }
}
void Game::clean()
{
    cerr << "Cleaning up game resources..." << std::endl;

    click_sound.freeSound();

    for (auto* threat : threats) delete threat;
    threats.clear();
    for (auto* item : lifeItems) delete item;
    lifeItems.clear();
    for (auto* item : scoreItems) delete item;
    scoreItems.clear();

    if (heart_texture) {
        SDL_DestroyTexture(heart_texture);
        heart_texture = nullptr;
    }
    if (timerFont) {
        TTF_CloseFont(timerFont);
        timerFont = nullptr;
    }

    if (g_screen) {
        SDL_DestroyRenderer(g_screen);
        g_screen = nullptr;
    }
    if (g_window) {
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}
