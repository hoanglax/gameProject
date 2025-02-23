#include "game.h"

bool initData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;
    else {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        g_window = SDL_CreateWindow("Meo beo thic mcdonald",
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
        std::cerr << "Renderer (g_screen) is NULL!" << std::endl;
        return false;
    }
    return success;
}

bool loadBackground()
{
    if (!g_background.loadImg("data/img/background.png", g_screen))
    {
        std::cerr << "Failed to load background image!\n";
        return false;
    }
    return true;
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
   // if (!loadBackground())
     //   return false;

    gameMap.loadMap("map/map01.dat");
    gameMap.loadTiles(g_screen);

    if (!player.loadImg("player/player_idle.png", g_screen))
        return false;
    player.set_clips();

    if (!loadThreat())
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
    renderThreat();
    updateThreat();
}

//threats
bool Game::loadThreat()
{
    if (!threat.loadImg("threats/car01.png", g_screen))
    {
        cerr << "Failed to load threats \n";
        return false;
    }
    threat.set_clips();
    threat.set_x_pos(LEFT);
    threat.set_y_pos(TILE_SIZE);
    threat.set_start_x(LEFT);
    threat.set_start_y(TILE_SIZE);
    threat.set_direction(1);

    return true;
}

void Game::renderThreat()
{
    float cam_y = player.get_y_pos() - SCREEN_HEIGHT/2;
    threat.Show(g_screen , cam_y);
}

void Game::updateThreat()
{
    Map map_data = gameMap.get_map();
    threat.DoPlayer(map_data);
}



//render
void Game::render()
{
    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    SDL_RenderClear(g_screen);

    g_background.Render(g_screen, NULL);
    gameMap.DrawMap(g_screen);
    player.Show(g_screen);
    renderThreat();

    SDL_RenderPresent(g_screen);
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

