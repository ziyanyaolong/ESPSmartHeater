#ifndef ZL_LVGL_UI_TOUCH_SCREEN_CALIBRATION_H
#define ZL_LVGL_UI_TOUCH_SCREEN_CALIBRATION_H

#include <Arduino.h>
#include <lvgl.h>

#include "Peripheral/LVGLDriver/LVGLDriver.h"

#include "Software/LVGLManager/API/LVGLUIAPI.h"

#include "IPC/MemoryShader/MemoryShader.h"

#include "ZLHSSystem/ZLHSSystem.h"

class TouchScreenCalibration : public LVGLUIAPI
{
public:
    TouchScreenCalibration();
    virtual ~TouchScreenCalibration();

private:
    lv_point_t lvPoint;

    lv_indev_t *indev = nullptr;
    LVGLDriver *driver = nullptr;

    uint16_t btnPSize = 15;

    lv_style_t styleBtnPA;
    lv_style_t styleBtnPB;

    lv_obj_t *btnStart = nullptr;

    lv_obj_t *btnP1 = nullptr;
    lv_obj_t *btnP2 = nullptr;
    lv_obj_t *btnP3 = nullptr;
    lv_obj_t *btnP4 = nullptr;

    lv_obj_t *labelECount = nullptr;

    double lcValue = 0.0f;

    LVGLDriver::TouchCalibrationState tcState = LVGLDriver::TouchCalibrationState::Close;
    LVGLDriver::TouchCalibrationState tcStateTemp = LVGLDriver::TouchCalibrationState::Close;

private:
    static void touchCStart(lv_event_t *e);

protected:
    virtual uint8_t init() override;
    virtual void setupUI(lv_obj_t *page) override;
    virtual void loop() override;
};

#endif