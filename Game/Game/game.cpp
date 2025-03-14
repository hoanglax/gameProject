#include "game.h"

bool initData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;
    else {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        g_window = SDL_CreateWindow("Mell bell don't like vegatable",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (g_window == NULL)
            success = false;
        else {
            g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
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
}

Game::~Game()
{
    clean();
}

bool Game::init()
{
    if (!initData())
        return false;
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

    return true;
}

void Game::run()
{
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
        player.HandelInputAction(g_event, g_screen);
    }
}

void Game::update()
{
    Map map_data = gameMap.get_map();
    player.SetMapXY(map_data.start_x, map_data.start_y);
    player.DoPlayer(map_data);
    gameMap.SetMap(map_data);
    updateThreats();
    renderThreats();
    checkCollisions();
    if (player.is_won())
    {
        cerr << "YOU WON" << endl;
    }
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
        //cout << "Threat X: " << threat->get_x_pos() << "\n";
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


//render
void Game::render()
{
    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    SDL_RenderClear(g_screen);

    gameMap.DrawMap(g_screen);
    player.Show(g_screen);
    renderThreats(); // Hiển thị threats
    
    SDL_RenderPresent(g_screen);
}

void Game::checkCollisions()
{
    SDL_Rect playerRect = player.GetRectFrame();

    for (auto threat : threats)
    {
        SDL_Rect threatRect = threat->GetRectFrame();

        if (CheckCollision(playerRect, threatRect))
        {
            cerr << "Meo beo da chet =[[\n";
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



void Game::clean()
{
    std::cout << "Cleaning up game resources..." << std::endl;

    if (g_screen)
    {
        SDL_DestroyRenderer(g_screen);
        g_screen = nullptr;
    }
    if (g_window)
    {
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
    }


    IMG_Quit();

    SDL_Quit();

    std::cout << "Game cleaned successfully!" << std::endl;
}
