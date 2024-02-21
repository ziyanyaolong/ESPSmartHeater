#ifndef ZL_LVGL_TOOLS_FL_SIZE_H
#define ZL_LVGL_TOOLS_FL_SIZE_H

#include <stdint.h>
#include <lvgl.h>

class FLSize
{
private:
    lv_coord_t _width = 0;
    lv_coord_t _height = 0;

public:
    FLSize();
    FLSize(const FLSize *size);
    FLSize(lv_coord_t w, lv_coord_t h);

    virtual ~FLSize();

    lv_coord_t width() const;
    lv_coord_t height() const;
};

#endif
