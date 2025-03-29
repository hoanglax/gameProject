#ifndef START_MENU_H_
#define START_MENU_H_
#include "CommonLib.h"
#include "CommonFunc.h"

enum ButtonType
{
	NONE = 0,
	START = 1,
	HOW_TO_PLAY = 2,
	EXIT = 3,
	BACK_TO_MENU = 4,
	RESTART = 5
};

class Button
{
public:
	SDL_Rect rect;
	string text;
	Button (int x, int y, int w, int h, string txt);
	void render(SDL_Renderer* renderer, TTF_Font* font);
};

class StartMenu : Object
{
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	vector<Button> buttons;

public:
	StartMenu(SDL_Renderer* ren);
	~StartMenu();
	
	bool init(const string& path);
	void render();
	int handleEvent();
};

class HowToPlayScreen : Object
{
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	Button backButton = Button(900 , 700 , 100 , 50 , "BACK");

public:
	HowToPlayScreen(SDL_Renderer* ren);
	~HowToPlayScreen();

	bool init(const string& path);
	void render();
	int handleEvent();
};

class GameOverScreen : Object
{
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	Button RestartButton = Button(400 , 300 , 200 , 50 , "RESTART");
	Button ExitButton = Button(400 , 400 , 200, 50, "EXIT");
public:
	GameOverScreen(SDL_Renderer* ren);
	~GameOverScreen();

	bool init(const string& path);
	void render();
	int handleEvent();
};

class WonScreen : Object
{
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	Button RestartButton = Button(400, 500, 200, 50, "RESTART");
	Button ExitButton = Button(400, 600, 200, 50, "EXIT");
public:
	WonScreen(SDL_Renderer* ren);
	~WonScreen();
	bool init(const string& path);
	void render();
	int handleEvent();
};

#endif // !START_MENU_H_

