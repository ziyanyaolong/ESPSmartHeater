#ifndef ZL_LVGL_TOOLS_FL_POINT_H
#define ZL_LVGL_TOOLS_FL_POINT_H

#include <lvgl.h>

class FLPoint
{
private:
    lv_point_t _point;

public:
    FLPoint();
    FLPoint(const FLPoint *point);
    FLPoint(const lv_point_t *point);
    FLPoint(lv_coord_t x, lv_coord_t y);

    virtual ~FLPoint();

    lv_coord_t x() const;
    lv_coord_t y() const;
};

#endif
