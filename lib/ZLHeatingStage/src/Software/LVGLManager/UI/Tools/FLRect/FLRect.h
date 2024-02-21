#ifndef ZL_LVGL_TOOLS_FL_RECT_H
#define ZL_LVGL_TOOLS_FL_RECT_H

#include "Software/LVGLManager/UI/Tools/FLPoint/FLPoint.h"
#include "Software/LVGLManager/UI/Tools/FLSize/FLSize.h"

class FLRect
{
private:
    lv_area_t _area;

public:
    FLRect();
    FLRect(const FLRect *rect);
    FLRect(const lv_area_t *rect);
    FLRect(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h);
    
    virtual ~FLRect();
};

#endif
