#ifndef ZL_LVGL_UI_API_H
#define ZL_LVGL_UI_API_H

#include <lvgl.h>

#include "Software/LVGLManager/API/WidgetAPI/WidgetAPI.h"

class LVGLUIAPI : public WidgetAPI
{
public:
    friend class LVGLManager;

    LVGLUIAPI() : WidgetAPI() {}
    virtual ~LVGLUIAPI() {}

    enum class APILoadStage
    {
        Empty = 0x00,
        Init,
        Loop
    };

private:
    APILoadStage lState = APILoadStage::Init;
    lv_obj_t *page = nullptr;

protected:
    virtual uint8_t init() = 0;
    virtual void setupUI(lv_obj_t *page) = 0;
    virtual void loop() {}
};

#endif