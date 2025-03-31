#ifndef LIFE_ITEM_H_
#define LIFE_ITEM_H_
#include "CommonLib.h"
#include "object.h"

class LifeItem : public Object
{
private:
    float x_pos_;
    float y_pos_;
    int width_frame_;
    int height_frame_;
    int frame_;

public:
    LifeItem();
    ~LifeItem();

    void set_x_pos(const float& xp) { x_pos_ = xp; }
    void set_y_pos(const float& yp) { y_pos_ = yp; }
    float get_x_pos() const { return x_pos_; }
    float get_y_pos() const { return y_pos_; }

    bool loadImg(string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des, float cam_y);
};

class ScoreItem : public Object
{

};
#endif // !LIFE_ITEM_H_
