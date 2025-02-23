#include "CommonFunc.h"

#include "game.h"

Object g_background;

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
	Game game;

	if (!game.init())
		return -1;

	if (!game.loadResources())
		return -1;

	game.run();

	return 0;
}
