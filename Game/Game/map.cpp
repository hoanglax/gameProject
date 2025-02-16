#include "map.h"

void GameMap::loadMap(const char* name)
{
	FILE* fp = NULL;
	fopen_s(&fp, name, "rb");
	if (fp == NULL)
		return;

	game_map.map_x = 0;
	game_map.map_y = 0;

	game_map.file_name = new char[strlen(name) + 1];
	strcpy_s(game_map.file_name, strlen(name) + 1, name);

	for (int i = 0 ; i < max_map_y ; i++) 
	{
		for (int j = 0; j < max_map_x ; j++)
		{
			fscanf_s(fp, "%d", &game_map.tile[i][j]);
			int val = game_map.tile[i][j];
			if (val > 0)
			{
				if (j > game_map.map_x)
				{
					game_map.map_x = j;
				}

				if (i > game_map.map_y) 
				{
					game_map.map_y = i;
				}
			}
		}
	}

	game_map.map_x = (game_map.map_x + 1) * TILE_SIZE;
	game_map.map_y = (game_map.map_y + 1) * TILE_SIZE;

	game_map.start_x = 0;
	game_map.start_y = 0;

	fclose(fp);
}


void GameMap::loadTiles(SDL_Renderer* screen)
{
	char file_img[20];
	FILE* fp = NULL;

	for (int i = 0; i < MAX_TILES; i++)
	{
		sprintf_s(file_img, "map/%d.png", i);

		fopen_s(&fp, file_img, "rb");
		if (fp == NULL) {
			continue;
		}

		fclose(fp);

		tile_mat[i].loadImg(file_img, screen);
	}
}

void GameMap::DrawMap(SDL_Renderer* screen)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	int map_x_ = 0;
	int map_y_ = 0;


	map_x_ = game_map.start_x / TILE_SIZE;
	x1 = (game_map.start_x % TILE_SIZE) * -1; 
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);


	map_y_ = game_map.start_y / TILE_SIZE;
	y1 = (game_map.start_y % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_WIDTH + (y1 == 0 ? 0 : TILE_SIZE);

	for (int i = y1; i < y2; i+= TILE_SIZE)
	{	
		map_x_ = game_map.start_x / TILE_SIZE;
		for (int j = x1; j < x2; j+= TILE_SIZE)
		{
			int val = game_map.tile[map_y_][map_x_];
			if (val > 0)
			{
				tile_mat[val].SetRect(j, i);
				tile_mat[val].Render(screen);
			}
			map_x_++;
		}
		map_y_++;
	}
}

GameMap::~GameMap()
{
	if (game_map.file_name != nullptr) {
		delete[] game_map.file_name;
		game_map.file_name = nullptr;
	}
}