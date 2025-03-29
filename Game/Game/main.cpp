#include "CommonFunc.h"
#include "CommonLib.h"
#include "game.h"
#include "menu.h"
#include "Music.h"


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0 || SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        cerr << "Error: SDL Init - " << SDL_GetError() << "\n";
        cerr << "Error: TTF Init - " << TTF_GetError() << "\n";
        cerr << "Error: Mixer Init - " << Mix_GetError() << "\n";
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! ERROR: %s", Mix_GetError());
        return -1;
    }

    Music bgMusic;
    if (!bgMusic.loadMusic("music/game_theme.mp3")) {
        return -1;
    }
    bgMusic.play(-1);
    
    SDL_Window* window = SDL_CreateWindow("Start menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!window || !renderer) return -1;

    StartMenu menu(renderer);
    if (!menu.init("data/Menu.png"))
    {
        cerr << "Error: init menu";
        return -1;
    }

    int action = NONE;
    while (true)
    {
        action = menu.handleEvent();

        SDL_RenderClear(renderer);
        menu.render();
        SDL_RenderPresent(renderer);

        if (action == START)
        {
            break;
        }
        else if (action == EXIT)
        {   
            return 0;
        }
        else if (action == HOW_TO_PLAY)
        {
            HowToPlayScreen how_to_play(renderer);
            if (!how_to_play.init("data/how_to_play.png"))
            {
                cerr << "Error: Init how to play screen in main";
                break;
            }

            int subAction = NONE;
            while (true)
            {
                subAction = how_to_play.handleEvent();

                SDL_RenderClear(renderer);
                how_to_play.render();
                SDL_RenderPresent(renderer);

                if (subAction == BACK_TO_MENU)
                    break;
                else if (subAction == EXIT)
                    return 0;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Game game;
    if (!game.init()) return -1;
    if (!game.loadResources()) return -1;

    game.run(); 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    //TTF_Quit();
    bgMusic.freeMusic();
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
