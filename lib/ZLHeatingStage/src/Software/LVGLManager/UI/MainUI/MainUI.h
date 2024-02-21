#ifndef ZL_LVGL_UI_MAIN_UI_H
#define ZL_LVGL_UI_MAIN_UI_H

#include "Software/LVGLManager/API/LVGLUIAPI.h"

#include "IPC/MemoryShader/MemoryShader.h"

#include "ZLHSSystem/ZLHSSystem.h"
#include "Software/LVGLManager/UI/Widgets/FLLabel/FLLabel.h"
#include "Peripheral/ZL_INA219/ZL_INA219.h"

class MainUI : public LVGLUIAPI
{
public:
    MainUI();
    virtual ~MainUI();

private:
    FLLabel *labelVoltage1 = nullptr;
    FLLabel *labelVoltage2 = nullptr;
    FLLabel *labelCurrent1 = nullptr;
    FLLabel *labelCurrent2 = nullptr;
    FLLabel *hPower1 = nullptr;
    FLLabel *hPower2 = nullptr;

    ZL_INA219* zl_INA219 = nullptr;

private:
protected:
    virtual uint8_t init() override;
    virtual void setupUI(lv_obj_t *page) override;
    virtual void loop() override;
};

#endif