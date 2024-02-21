#include "FLPoint.h"

FLPoint::FLPoint()
{
    this->_point.x = 0;
    this->_point.y = 0;
}

FLPoint::FLPoint(const FLPoint *point)
{
    this->_point.x = point->_point.x;
    this->_point.y = point->_point.y;
}

FLPoint::FLPoint(const lv_point_t *point)
{
    this->_point.x = point->x;
    this->_point.y = point->y;
}

FLPoint::FLPoint(lv_coord_t x, lv_coord_t y)
{
    this->_point.x = x;
    this->_point.y = y;
}

FLPoint::~FLPoint()
{
}

lv_coord_t FLPoint::x() const
{
    return this->_point.x;
}

lv_coord_t FLPoint::y() const
{
    return this->_point.y;
}

