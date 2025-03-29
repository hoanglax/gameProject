#include "Music.h"

Music::Music() : gMusic(nullptr) {}

Music::~Music() {
    freeMusic();
}

bool Music::loadMusic(const char* path)
{
    freeMusic(); 

    gMusic = Mix_LoadMUS(path);
    if (gMusic == nullptr) {
        SDL_Log("Could not load music! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }
    return true;
}

void Music::play(int loop)
{
    if (gMusic == nullptr) return;

    if (Mix_PlayMusic(gMusic, loop) == -1) {
        SDL_Log("Failed to play music: %s", Mix_GetError());
    }
}

void Music::pause()
{
    if (Mix_PlayingMusic()) {
        Mix_PauseMusic();
    }
}

void Music::resume()
{
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void Music::stop()
{
    Mix_HaltMusic();
}

void Music::freeMusic()
{
    if (gMusic != nullptr) {
        Mix_FreeMusic(gMusic);
        gMusic = nullptr;
    }
}
