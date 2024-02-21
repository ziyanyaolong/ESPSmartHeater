#ifndef ZL_LVGL_UI_MAIN_UI_H
#define ZL_LVGL_UI_MAIN_UI_H

#include "Software/LVGLManager/API/LVGLUIAPI.h"

#include "IPC/MemoryShader/MemoryShader.h"

#include "ZLHSSystem/ZLHSSystem.h"
#include "Software/LVGLManager/UI/Widgets/FLLabel/FLLabel.h"

class MainUI : public LVGLUIAPI
{
public:
    MainUI();
    virtual ~MainUI();

private:

private:

protected:
    virtual uint8_t init() override;
    virtual void setupUI(lv_obj_t *page) override;
    virtual void loop() override;
};

#endif