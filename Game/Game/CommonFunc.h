#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include "CommonLib.h"
#include "object.h"

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//Screen
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP = 64;

#define TILE_SIZE 64
#define BLANK_TILE 0
#define BLOCKED_TILE_1 19
#define BLOCKED_TILE_2 20
#define max_map_x 16
#define max_map_y 80
#define WON_TILE 4

//graphic
#define FPS 24

//player
#define PLAYER_MOVE_SPEED 4
#define PLAYER_RUN_SPEED 6
#define PLAYER_LIFE 3

//threats
#define LEFT 0
#define RIGHT 960
#define LEFT_THREAT_NUMBER 16

typedef struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;
	int run_;
};

typedef struct Map
{
	int start_x;
	int start_y;

	int map_x;
	int map_y;

	int tile[max_map_y][max_map_x];
	char* file_name;
};

#endif
