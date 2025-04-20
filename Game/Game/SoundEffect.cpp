#include "SoundEffect.h"

SoundEffect::SoundEffect()
{
	sound = nullptr;
}

SoundEffect::~SoundEffect()
{
	freeSound();
}

bool SoundEffect::loadSound(const string& path)
{
	freeSound();
	sound = Mix_LoadWAV(path.c_str());
	if (!sound)
	{
		cerr << "Fail to load sound";
		return false;
	}
	return true;
}

void SoundEffect::play(int loops) const 
{
	if (sound) {
		Mix_PlayChannel(-1, sound, loops);
	}
}

void SoundEffect::freeSound() 
{
	if (sound) {
		Mix_FreeChunk(sound);
		sound = nullptr;
	}
}