#ifndef  MAP_H
#define MAP_H

#include "CommonFunc.h"
#include "object.h"
#include "MainObject.h"


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
	Map get_map() const { return game_map; }
	void SetMap(Map& map_data) { game_map = map_data; }

private:
	TileMat tile_mat[MAX_TILES];
	Map game_map;
	int current_map_index = 0;
};

#endif // ! MAP_H ;
