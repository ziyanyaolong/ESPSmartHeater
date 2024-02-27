#ifndef ZL_LVGL_UI_MAIN_UI_H
#define ZL_LVGL_UI_MAIN_UI_H

#include "Software/LVGLManager/API/LVGLUIAPI.h"

#include "IPC/MemoryShader/MemoryShader.h"

#include "ZLHSSystem/ZLHSSystem.h"
#include "Software/LVGLManager/UI/Widgets/FLLabel/FLLabel.h"
#include "Software/LVGLManager/UI/Widgets/FLWidget/FLWidget.h"
#include "Software/LVGLManager/UI/Widgets/FLLayout/FLLayoutFlex/FLLayoutFlex.h"

class MainUI : public LVGLUIAPI
{
public:
    MainUI();
    virtual ~MainUI();

private:
	FLLayoutFlex* fLayoutPage = nullptr;

    FLLayoutFlex* fLayoutVoltage = nullptr;
	FLLayoutFlex* fLayoutCurrent = nullptr;
	FLLayoutFlex* fLayoutPower = nullptr;

    FLWidget* widgetVoltage = nullptr;
	FLWidget* widgetCurrent = nullptr;
	FLWidget* widgetPower = nullptr;

    FLLabel *labelVoltage1 = nullptr;
    FLLabel *labelVoltage2 = nullptr;
    FLLabel *labelCurrent1 = nullptr;
    FLLabel *labelCurrent2 = nullptr;
    FLLabel *hPower1 = nullptr;
    FLLabel *hPower2 = nullptr;

    double eValue = 0.0f;
private:
protected:
    virtual std::string getName() override { return std::string(ZLHS_EX_TO_STRING(MainUI)); }

    virtual uint8_t init() override;
    virtual void setupUI(lv_obj_t *page) override;
    virtual void loop() override;
};

#endif