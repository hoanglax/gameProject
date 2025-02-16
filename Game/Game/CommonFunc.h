#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include "CommonLib.h"
#include "object.h"

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
extern Object g_background;
//Screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;


#endif
