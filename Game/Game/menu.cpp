#include "menu.h"

Button::Button(int x , int y , int h , int w , string txt)
{
	rect = { x , y , h , w };
	text = txt;
}

void Button::render(SDL_Renderer* renderer, TTF_Font* font)
{   
    SDL_SetRenderDrawColor(renderer, 248, 243, 217, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Color textColor = { 0, 0, 0 };  
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = { rect.x + 20, rect.y + 10, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

StartMenu::StartMenu(SDL_Renderer* ren)
{
    renderer = ren;
    font = nullptr;
}
StartMenu::~StartMenu()
{
    if (font)
    {
        cerr << "StartMenu Destructor: Closing font at " << font << "\n";
        TTF_CloseFont(font);
        font = nullptr;
    }
}

bool StartMenu::init(const string& path)
{
    if (!loadImg(path, renderer))
    {
        cout << "load menu img error";
        return false;
    }
    
    font = TTF_OpenFont("data/font.ttf", 36);
    if (!font)
    {
        cout << "Error: load font";
        return false;
    }

    buttons.push_back(Button(400, 450, 200, 50, "START"));
    buttons.push_back(Button(400, 550, 200, 50, "HOW TO PLAY"));
    buttons.push_back(Button(400, 650, 200, 50, "EXIT"));

    return true;
}
void StartMenu::render()
{
    Render(renderer, nullptr);
    for (auto& button : buttons)
    {
        button.render(renderer, font);
    }
}
int StartMenu::handleEvent()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) return EXIT;

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int x = e.button.x;
            int y = e.button.y;
            for (int i = 0; i < buttons.size(); i++)
            {
                if (x >= buttons[i].rect.x && x <= buttons[i].rect.x + buttons[i].rect.w &&
                    y >= buttons[i].rect.y && y <= buttons[i].rect.y + buttons[i].rect.h)
                {
                    return i + 1;
                }
            }
        }
    }
    return NONE;
}

HowToPlayScreen::HowToPlayScreen(SDL_Renderer* ren)
{
    renderer = ren;
    font = nullptr;
}
HowToPlayScreen::~HowToPlayScreen()
{
    if (font)
        TTF_CloseFont(font);
}
bool HowToPlayScreen::init(const string& path)
{
    if (!loadImg(path, renderer))
    {
        cout << "Error: load how to play screen\n";
        return false;
    }

    font = TTF_OpenFont("data/font.ttf", 36);
    if (!font)
    {
        cout << "Error: load font";
        return false;
    }

    return true;
}
void HowToPlayScreen::render()
{
    Render(renderer, nullptr);
    backButton.render(renderer, font);
}
int HowToPlayScreen::handleEvent()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) return EXIT;

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int x = e.button.x;
            int y = e.button.y;
            if (x >= backButton.rect.x && x <= backButton.rect.x + backButton.rect.w &&
                y >= backButton.rect.y && y <= backButton.rect.y + backButton.rect.h)
            {
                return BACK_TO_MENU;
            }
        }
    }
    return NONE;
}

GameOverScreen::GameOverScreen(SDL_Renderer* ren)
{
    renderer = ren;
    font = nullptr;
}
GameOverScreen::~GameOverScreen()
{
    if (font)
        TTF_CloseFont(font);
}
bool GameOverScreen::init(const string& path)
{
    if (renderer == nullptr)
    {
        cerr << "Error: Renderer is null in GameOverScreen::init\n";
        return false;
    }
    if (!loadImg(path, renderer))
    {
        cerr << "Error: load game over screen - " << path << endl;
        return false;
    }

    font = TTF_OpenFont("data/font.ttf", 36);
    if (!font)
    {
        cerr << "Error: load font - " << TTF_GetError() << endl;
        return false;
    }

    return true;
}
void GameOverScreen::render()
{
    Render(renderer, nullptr);
    RestartButton.render(renderer, font);
    ExitButton.render(renderer, font);
}
int GameOverScreen::handleEvent()  
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) return EXIT;

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int x = e.button.x;
            int y = e.button.y;
            cout << "Mouse click: " << x << " " << y << endl;
            if (x >= RestartButton.rect.x && x <= RestartButton.rect.x + RestartButton.rect.w &&
                y >= RestartButton.rect.y && y <= RestartButton.rect.y + RestartButton.rect.h)
            {
                cout << "Restart clicked" << endl;
                return RESTART;
            }
            else if (x >= ExitButton.rect.x && x <= ExitButton.rect.x + ExitButton.rect.w &&
                y >= ExitButton.rect.y && y <= ExitButton.rect.y + ExitButton.rect.h)
            {
                cout << "Exit clicked" << endl;
                return EXIT;
            }
        }
    }
    return NONE;
}
WonScreen::WonScreen(SDL_Renderer* ren)
{
    renderer = ren;
    font = nullptr;
}
WonScreen::~WonScreen()
{
    if (font)
    {
        TTF_CloseFont(font);
    }
}
bool WonScreen::init(const string& path)
{
    if (renderer == nullptr)
    {
        cerr << "Renderer is null (int won screen)";
        return false;
    }
    if (!loadImg(path, renderer))
    {
        cerr << "Fail to load won screen img";
        return false;
    }
    
    font = TTF_OpenFont("data/font.ttf", 36);
    if (!font)
    {
        cerr << "Fail to load font in won screen";
        return false;
    }
    return true;
}

void WonScreen::render()
{
    Render(renderer, nullptr);
    RestartButton.render(renderer, font);
    ExitButton.render(renderer, font);
}

int WonScreen::handleEvent()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) return EXIT;

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int x = e.button.x;
            int y = e.button.y;
            cout << "Mouse click: " << x << " " << y << endl;
            if (x >= RestartButton.rect.x && x <= RestartButton.rect.x + RestartButton.rect.w &&
                y >= RestartButton.rect.y && y <= RestartButton.rect.y + RestartButton.rect.h)
            {
                cout << "Restart clicked" << endl;
                return RESTART;
            }
            else if (x >= ExitButton.rect.x && x <= ExitButton.rect.x + ExitButton.rect.w &&
                y >= ExitButton.rect.y && y <= ExitButton.rect.y + ExitButton.rect.h)
            {
                cout << "Exit clicked" << endl;
                return EXIT;
            }
        }
    }
    return NONE;
}
