#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H

#include "CommonLib.h"
#include "CommonFunc.h"
#include "object.h"
#include "ImpTimer.h"
#include "SoundEffect.h"

class MainObject : public Object
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_RIGHT = 0,
		WALK_LEFT = 1,
		WALK_UP = 2,
		WALK_DOWN = 3,
		RUN = 4
	};

	bool loadImg(string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandelInputAction(SDL_Event events, SDL_Renderer* screen, SoundEffect& move_sound);
	void set_clips();
	//move
	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
	void CenterEntityOnMap(Map& map_data);

	float get_y_pos() { return y_pos_; }

	bool is_won() const { return is_won_; }
	void resetPosition();

	SDL_Rect getHitboxRect();

	bool is_hit = false;
	ImpTimer hitTimer;

	void take_damage()
	{
		is_hit = true;
		hitTimer.start();
	}

	bool is_picked = false;
	ImpTimer pickedTimer;
	void picked_item()
	{
		is_picked = true;
		pickedTimer.start();
	}

private:
	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];
	Input input_type_;
	int frame_;
	int status_;

	int map_x_;
	int map_y_;
	bool isRunning;
	bool is_won_ = false;
	
};

#endif 
