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
        InitToDisplayUI,
        Init,
        Loop
    };

    typedef uint32_t TimeType;

private:
    APILoadStage lState = APILoadStage::Init;
    lv_obj_t *page = nullptr;
    TimeType oldTime = 0;
    TimeType delayTimeMS = 500;

protected:
    void setOnlyDisplayUI(bool value) { value ? lState = APILoadStage::InitToDisplayUI : lState = APILoadStage::Init; }

    virtual std::string getName() = 0;

    virtual uint8_t init() = 0;
    virtual void setupUI(lv_obj_t *page) = 0;
    virtual void loop() {}
};

#endif