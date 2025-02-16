#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include "CommonLib.h"
#include "object.h"

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
extern Object g_background;
//Screen
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP = 64;

#define TILE_SIZE 64
#define max_map_x 16
#define max_map_y 40


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
