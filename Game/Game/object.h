#ifndef OBJECT_H
#define OBJECT_H
#include "CommonLib.h"
#include "CommonFunc.h"

class Object
{
protected:

	SDL_Texture* p_object;
	SDL_Rect rect_;

	int width_frame_;
	int height_frame_;
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

	virtual bool loadImg(string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void Free();

	SDL_Rect GetRectFrame();

};

#endif 
