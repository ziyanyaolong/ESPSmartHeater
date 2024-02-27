#ifndef ZL_LVGL_WIDGET_FLLABEL_H
#define ZL_LVGL_WIDGET_FLLABEL_H

#include <string>

#include "Software/LVGLManager/API/WidgetAPI/WidgetAPI.h"

class FLLabel : public WidgetAPI
{
private:
    lv_style_t *style;

private:
    void initStyle();

public:
    FLLabel(WidgetAPI *parent = nullptr);
    FLLabel(lv_obj_t *parent);

    virtual ~FLLabel();

    void setText(const std::string& text);
    std::string getText();
};

#endif