#ifndef OBJECT_H
#define OBJECT_H
#include "CommonLib.h"

class Object
{
protected:

	SDL_Texture* p_object;
	SDL_Rect rect_;

public:
	Object();
	~Object();
	void SetRect(const int& x, const int& y) 
	{
		rect_.x = x; 
		rect_.y = y;
	}
	SDL_Rect GetRect() const
	{
		return rect_;
	}
	SDL_Texture* getObject() const 
	{
		return p_object;
	}

	bool loadImg(string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void Free();

};

#endif 
