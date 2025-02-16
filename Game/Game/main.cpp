#include "CommonFunc.h"
#include "map.h"

Object g_background;

bool initData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;
	else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		g_window = SDL_CreateWindow("Jack qua duong",
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
	if (!g_background.loadImg("data/img/background2.png", g_screen))
	{
		std::cerr << "Failed to load background image!\n";
		return false;
	}
	return true;
}

void Close() {
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (initData() == false)
		return -1;

	if (loadBackground() == false)
		return -1;


	GameMap game_map;

	game_map.loadMap("map/map01.dat");
	game_map.loadTiles(g_screen);

	bool isQuit = false;
	while (!isQuit)
	{
		while (SDL_PollEvent(&g_event) != 0) 
		{
			if (g_event.type == SDL_QUIT)
			{
				isQuit = true;
			}
		}

		SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);
		game_map.DrawMap(g_screen);



		SDL_RenderPresent(g_screen);
	}

	return 0;
}