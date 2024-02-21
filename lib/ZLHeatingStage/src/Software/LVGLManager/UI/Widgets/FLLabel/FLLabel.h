#ifndef ZL_LVGL_UI_FLLABEL_H
#define ZL_LVGL_UI_FLLABEL_H

#include <string>

#include "Software/LVGLManager/API/WidgetAPI/WidgetAPI.h"

class FLLabel : public WidgetAPI
{
private:
    lv_obj_t *label;

public:
    FLLabel(WidgetAPI *parent = nullptr);
    FLLabel(lv_obj_t *parent);

    virtual ~FLLabel();

    void setText(std::string text);
    std::string getText();
};

#endif