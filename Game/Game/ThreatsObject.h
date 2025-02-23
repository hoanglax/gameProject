#ifndef  THREATSOBJECT_H
#define THREATS_OBJECT

#include "object.h"
#include "map.h"
#define THREAT_FRAME_NUM 8
#define THREAT_SPEED 10

class ThreatObject : public Object
{
public:
	ThreatObject();
	~ThreatObject();

	void set_x_val(float xVal) { x_val_ = xVal; }
	void set_y_val(float yVal) { y_val_ = yVal; }
	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }
	void set_start_x(float x) { start_x_ = x; };
	void set_start_y(float y) { start_y_ = y; };
	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }

	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }
	float get_map_x() { return map_x_; }
	float get_map_y() { return map_y_; }

	void set_clips();
	bool loadImg(string path , SDL_Renderer* screen);
	void Show(SDL_Renderer* des , float cam_y);
	void set_direction(int dir);
	int get_direction() const { return direction_; }

	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }

	void DoPlayer(Map& gMap);

	vector<string> threats_list = {
		"car01LEFT.png",
		"car01RIGHT.png"
	};
private:
	float map_x_;
	float map_y_;

	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;
	int direction_; //1 for the right , -1 for the left

	float start_x_;
	float start_y_;

	SDL_Rect frame_clip_[THREAT_FRAME_NUM];
	int width_frame_;
	int height_frame_;
	int frame_;
	int come_back_time;
};

#endif // ! THREATSOBJECT_H
