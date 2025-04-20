#ifndef SOUND_EFFECT_H_
#define SOUND_EFFECT_H_
#include "CommonLib.h"

class SoundEffect
{
public:
	SoundEffect();
	~SoundEffect();

	bool loadSound(const string& path);
	void play(int loops = 0) const;
	void freeSound();

private:
	Mix_Chunk* sound;
};

#endif
