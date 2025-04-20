#ifndef START_MENU_H_
#define START_MENU_H_
#include "CommonLib.h"
#include "CommonFunc.h"
#include "SoundEffect.h"

enum ButtonType
{
	NONE = 0,
	START = 1,
	HOW_TO_PLAY = 2,
	EXIT = 3,
	BACK_TO_MENU = 4,
	RESTART = 5,
	RESUME = 6,
};

class Button
{
public:
	SDL_Rect rect;
	string text;

	Button(int x, int y, int w, int h, string txt);

	void render(SDL_Renderer* renderer, TTF_Font* font , int mouseX , int mouseY);
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
	int handleEvent(SoundEffect& click);
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
	int handleEvent(SoundEffect& click);
};

class GameOverScreen : Object
{
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	Button RestartButton = Button(400 , 400 , 200 , 50 , "RESTART");
	Button ExitButton = Button(400 , 500 , 200, 50, "EXIT");
public:
	GameOverScreen(SDL_Renderer* ren);
	~GameOverScreen();

	bool init(const string& path);
	void render();
	int handleEvent(SoundEffect& click);
};

class WonScreen : Object
{
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	Button RestartButton = Button(400, 600, 200, 50, "RESTART");
	Button ExitButton = Button(400, 700, 200, 50, "EXIT");
public:
	WonScreen(SDL_Renderer* ren);
	~WonScreen();
	bool init(const string& path);
	void render();
	int handleEvent(SoundEffect& click);
};

class PauseScreen : Object
{
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	Button ContinueButton = Button(400, 350, 200, 50, "CONTINUE");
	Button ExitButton = Button(400, 450, 200, 50, "EXIT");
public:
	PauseScreen(SDL_Renderer* ren);
	~PauseScreen();
	bool init(const string& path);
	void render();
	int handleEvent(SoundEffect& click);
};
#endif // !START_MENU_H_

