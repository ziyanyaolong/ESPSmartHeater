#include "FLRect.h"

FLRect::FLRect()
{
    this->_area.x1 = 0;
    this->_area.y1 = 0;
    this->_area.x2 = 0;
    this->_area.y2 = 0;
}

FLRect::FLRect(const FLRect *rect)
{
    lv_area_copy(&(this->_area), &(rect->_area));
}

FLRect::FLRect(const lv_area_t *rect)
{
    lv_area_copy(&(this->_area), rect);
}

FLRect::FLRect(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h)
{
    this->_area.x1 = x;
    this->_area.y1 = y;
    this->_area.x2 = w;
    this->_area.y2 = h;
}

FLRect::~FLRect()
{
}