#include "MainObject.h"


MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = SCREEN_WIDTH/2 - TILE_SIZE;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;

	isRunning = false;

	width_frame_ = 0;
	height_frame_ = 0;
	status_ = -1;

	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	input_type_.run_ = 0;

	map_x_ = 0;
	map_y_ = 0;
}

MainObject::~MainObject() 
{

}

bool MainObject::loadImg(string path, SDL_Renderer* screen)
{
	bool ret = Object::loadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w/8;
		height_frame_ = rect_.h;
	}
	return ret;
}

void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		frame_clip_[2].x = 2*width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		frame_clip_[3].x = 3 * width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;

		frame_clip_[4].x = 4 * width_frame_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = width_frame_;
		frame_clip_[4].h = height_frame_;

		frame_clip_[5].x = 5 * width_frame_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = width_frame_;
		frame_clip_[5].h = height_frame_;

		frame_clip_[6].x = 6 * width_frame_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = width_frame_;
		frame_clip_[6].h = height_frame_;

		frame_clip_[7].x = 7 * width_frame_;
		frame_clip_[7].y = 0;
		frame_clip_[7].w = width_frame_;
		frame_clip_[7].h = height_frame_;
	}
}

void MainObject::Show(SDL_Renderer* des) 
{
	if (status_ == WALK_LEFT) 
	{
		loadImg("player/player_left.png", des);
	}
	else if (status_ == WALK_RIGHT)
	{
		loadImg("player/player_right.png", des);
	}
	else if (status_ == WALK_UP)
	{
		loadImg("player/player_back.png", des);
	}
	else if (status_ == WALK_DOWN)
	{
		loadImg("player/player_front.png", des);
	}
	else
	{
		loadImg("player/player_idle.png", des);
	}

	if (input_type_.left_ == 1 ||
		input_type_.right_ == 1 ||
		input_type_.up_ == 1 ||
		input_type_.down_ == 1) 
	{
		frame_++;
	}
	else 
	{
		frame_ = 0;
	}

	if (frame_ >= 8)
	{
		frame_ = 0;
	}

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x , rect_.y , width_frame_ , height_frame_ };

	SDL_RenderCopy(des, p_object, current_clip, &renderQuad);
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		if (events.key.keysym.sym == SDLK_d)
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;

			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		if (events.key.keysym.sym == SDLK_a)
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;

			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
		}
		if (events.key.keysym.sym == SDLK_w)
		{
			status_ = WALK_UP;
			input_type_.up_ = 1;

			input_type_.left_ = 0;
			input_type_.right_ = 0;
			input_type_.down_ = 0;
		}
		if (events.key.keysym.sym == SDLK_s)
		{
			status_ = WALK_DOWN;
			input_type_.down_ = 1;

			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.right_ = 0;
		}
		if (events.key.keysym.sym == SDLK_LSHIFT)
		{
			status_ = RUN;
			isRunning = true;
			input_type_.run_ = 1;
		}
	}
	if (events.type == SDL_KEYUP)
	{
		if (events.key.keysym.sym == SDLK_d)
		{
			input_type_.right_ = 0;
		}
		if (events.key.keysym.sym == SDLK_a)
		{
			input_type_.left_ = 0;
		}
		if (events.key.keysym.sym == SDLK_w)
		{
			input_type_.up_ = 0;
		}
		if (events.key.keysym.sym == SDLK_s)
		{
			input_type_.down_ = 0;
			status_ = -1;
		}
		if (events.key.keysym.sym == SDLK_LSHIFT)
		{	
			isRunning = false;
			input_type_.run_ = 0;
		}
	}
}

void MainObject::DoPlayer(Map& map_data)
{
	x_val_ = 0;
	y_val_ = 0;

	bool movingHorizontally = false;
	bool movingVertically = false;

	if (input_type_.left_ == 1)
	{
		x_val_ -= PLAYER_MOVE_SPEED;
		movingHorizontally = true;
	}
	if (input_type_.right_ == 1)
	{
		x_val_ += PLAYER_MOVE_SPEED;
		movingHorizontally = true;
	}
	if (input_type_.up_ == 1)
	{
		y_val_ -= PLAYER_MOVE_SPEED;
		movingVertically = true;
	}
	if (input_type_.down_ == 1)
	{
		y_val_ += PLAYER_MOVE_SPEED;
		movingVertically = true;
	}

	if (movingHorizontally && movingVertically)
	{
		x_val_ /= sqrt(2.0);
		y_val_ /= sqrt(2.0);
	}

	if (input_type_.run_ == 1)
	{

		if (input_type_.right_)
		{	
			status_ = WALK_RIGHT;
			x_val_ += PLAYER_RUN_SPEED;
		}
		if (input_type_.left_)
		{
			status_ = WALK_LEFT;
			x_val_ -= PLAYER_RUN_SPEED;
		}
		if (input_type_.down_)
		{
			status_ = WALK_DOWN;
			y_val_ += PLAYER_RUN_SPEED;
		}
		if (input_type_.up_)
		{
			status_ = WALK_UP;
			y_val_ -= PLAYER_RUN_SPEED;
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	CheckToMap(map_data);

	CenterEntityOnMap(map_data);
}

void MainObject::CheckToMap(Map& map_data)
{
	if (x_pos_ < 0)
	{
		x_pos_ = 0;  
	}
	else if (x_pos_ + width_frame_ > (max_map_x*TILE_SIZE))
	{
		x_pos_ = (max_map_x * TILE_SIZE) - width_frame_;
	}

	if (y_pos_ < 0)
	{
		y_pos_ = 0; 
	}
	else if (y_pos_ + height_frame_ > (max_map_y * TILE_SIZE))
	{
		y_pos_ = (max_map_y * TILE_SIZE) - height_frame_;
	}
}
void MainObject::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x = x_pos_ - (SCREEN_WIDTH / 2);
	if (map_data.start_x < 0)
	{
		map_data.start_x = 0;
	}
	else if (map_data.start_x + SCREEN_WIDTH >= map_data.map_x)
	{
		map_data.start_x = map_data.map_x - SCREEN_WIDTH;
	}

	map_data.start_y = y_pos_ - TILE_SIZE;
	if (map_data.start_y < 0)
	{
		map_data.start_y = 0;
	}
	else if (map_data.start_y + SCREEN_HEIGHT >= map_data.map_y)
	{
		map_data.start_y = map_data.map_y - SCREEN_HEIGHT;
	}
}


