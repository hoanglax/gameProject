#include "ThreatsObject.h"

ThreatObject::ThreatObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;
	come_back_time = 0;
	frame_ = 0;

	start_x_ = 0.0;
	start_y_ = 0.0;
	direction_ = 1;
}

ThreatObject::~ThreatObject()
{

}

bool ThreatObject::loadImg(string path, SDL_Renderer* screen)
{
	bool ret = Object::loadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w /THREAT_FRAME_NUM;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ThreatObject::set_clips()
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

		frame_clip_[2].x = 2 * width_frame_;
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

void ThreatObject::Show(SDL_Renderer* des , float cam_y)
{
	if (come_back_time == 0)
	{
		rect_.x = static_cast<int>(x_pos_);
		rect_.y =static_cast<int>(y_pos_ - cam_y);

		frame_++;
		if (frame_ >= 8)
		{
			frame_ = 0;
		}

		SDL_Rect* currentClip = &frame_clip_[frame_];
		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
		SDL_RenderCopy(des, p_object, currentClip, &renderQuad);
	}
}


void ThreatObject::DoPlayer(Map& gMap)
{
	if (come_back_time == 0)
	{	
		x_val_ = THREAT_SPEED;
		x_pos_ += x_val_ * direction_;
		y_val_ = 0;

		cout << "Threat X Position: " << x_pos_ << ", Direction: " << direction_ << endl;

		if (x_pos_ < 0 || x_pos_ >= SCREEN_WIDTH)
		{
			come_back_time = 60;
		}
	}
	else if (come_back_time > 0)
	{
		come_back_time--;
		if (come_back_time == 0)
		{
			x_pos_ = start_x_;
			x_val_ = 0;
			y_val_ = 0;
		}
	}
}

void ThreatObject::set_direction(int dir) {
	direction_ = dir;
}