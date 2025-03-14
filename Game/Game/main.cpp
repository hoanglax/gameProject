#include "CommonFunc.h"

#include "game.h"

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
