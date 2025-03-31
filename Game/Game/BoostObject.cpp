#include "BoostObject.h"

LifeItem::LifeItem() 
{
    x_pos_ = 0;
    y_pos_ = 0;
    frame_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
}

LifeItem::~LifeItem()
{
}

bool LifeItem::loadImg(string path, SDL_Renderer* screen)
{   
    bool ret = Object::loadImg(path, screen);
    if (ret)
    {
        width_frame_ = rect_.w;
        height_frame_ = rect_.h;
    }

    return ret;
}


void LifeItem::Show(SDL_Renderer* des, float cam_y)
{
    rect_.x = static_cast<int>(x_pos_);
    rect_.y = static_cast<int>(y_pos_ - cam_y);

    SDL_RenderCopy(des, p_object, NULL, &rect_);
}

