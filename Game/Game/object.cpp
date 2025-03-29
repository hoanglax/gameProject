#include "object.h"

Object::Object()
{
	p_object = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
}
Object::~Object()
{
	Free();
}

bool Object::loadImg(string path, SDL_Renderer* screen)
{
	Free(); 

	SDL_Surface* loadSurface = IMG_Load(path.c_str());

	if (loadSurface == NULL)
	{
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
		std::cerr << "Check if the file exists: " << path << std::endl;
		return false;
	}

	p_object = SDL_CreateTextureFromSurface(screen, loadSurface);
	
	if (p_object == NULL)
	{
		std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		rect_.w = loadSurface->w;
		rect_.h = loadSurface->h;
		width_frame_ = (rect_.w / 8) - (TILE_SIZE / 4);
		height_frame_ = (rect_.h) - 16 - (TILE_SIZE / 4);
	}

	SDL_FreeSurface(loadSurface);
	return p_object != NULL;
}



void Object::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
	SDL_Rect renderquad = { rect_.x , rect_.y , rect_.w , rect_.h };
	SDL_RenderCopy(des, p_object, clip, &renderquad);
}


void Object::Free()
{
	if (p_object != NULL)
	{
		SDL_DestroyTexture(p_object);
		p_object = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}

SDL_Rect Object::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}