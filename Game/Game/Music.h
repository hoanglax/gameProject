#ifndef MUSIC_H_
#define MUSIC_H_

#include "CommonLib.h"
#include <SDL_mixer.h>

class Music
{
public:
    Music();
    ~Music();

    bool loadMusic(const char* path);
    void play(int loop = -1);
    void pause();
    void resume();
    void stop();
    void freeMusic();

private:
    Mix_Music* gMusic;
};

#endif // !MUSIC_H_
