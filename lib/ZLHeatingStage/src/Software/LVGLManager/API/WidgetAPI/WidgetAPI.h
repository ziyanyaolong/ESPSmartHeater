#ifndef ZL_WIDGET_API_H
#define ZL_WIDGET_API_H

#include <string>
#include <lvgl.h>

#include "Software/LVGLManager/UI/Tools/FLRect/FLRect.h"

class WidgetAPI
{
private:
    lv_obj_t *_parent = nullptr;
    lv_obj_t *obj = nullptr;

    lv_style_t _style;

protected:
    WidgetAPI(WidgetAPI *parent = nullptr);
    WidgetAPI(lv_obj_t *parent);
    
    virtual ~WidgetAPI();

    void setLVGLObj(lv_obj_t* obj);
    lv_obj_t* getLVGLObj();

public:
    void setSize(const FLSize &size);
    void setSize(lv_coord_t w, lv_coord_t h);

    void setPoint(const FLPoint &point);
    void setPoint(lv_coord_t x, lv_coord_t y);

    lv_obj_t* parent();

    FLSize size();
    FLPoint point();
};

#endif