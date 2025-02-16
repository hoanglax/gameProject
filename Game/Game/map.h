#ifndef  MAP_H
#define MAP_H

#include "CommonFunc.h"
#include "object.h"


#define MAX_TILES 20

class TileMat : public Object
{
public:
	TileMat() { ; };
	~TileMat() { ; };
};

class GameMap
{
public:
	GameMap() { ; }
	~GameMap();

	void loadMap(const char* name);
	void loadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);
private:
	Map game_map;
	TileMat tile_mat[MAX_TILES];
};

#endif // ! MAP_H
