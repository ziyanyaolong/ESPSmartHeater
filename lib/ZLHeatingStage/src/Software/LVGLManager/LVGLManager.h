#ifndef ZL_LVGL_MANAGER_H
#define ZL_LVGL_MANAGER_H

#include <lvgl.h>
#include <list>

#include "Software/LVGLManager/API/LVGLUIAPI.h"

#include "Peripheral/DisplayDriver/DisplayDriver.h"
#include "Peripheral/LVGLDriver/LVGLDriver.h"
#include "Peripheral/EncoderDriver/EncoderDriver.h"
#include "Peripheral/ZL_Buzzer/ZL_Buzzer.h"

#include "Software/LVGLManager/UI/TouchScreenCalibration/TouchScreenCalibration.h"
#include "Software/LVGLManager/UI/MainUI/MainUI.h"

#include "IPC/MemoryShader/MemoryShader.h"

#include "ZLHSSystem/ZLHSSystem.h"

#include "CodeGenerate/CodeGenerate.h"
#include "API/SoftwareAPI.h"

class LVGLManager : public SoftwareAPI
{
public:
    LVGLManager();
    virtual ~LVGLManager();

private:
    std::list<LVGLUIAPI *> uiList;

    LVGLDriver *driver = nullptr;
    DisplayDriver *baseDriver = nullptr;
    EncoderDriver *encoderDriver = nullptr;
    ZL_Buzzer *zl_Buzzer = nullptr;

    double eValue = 0.0f;

private:
protected:
    virtual int init() override;
    virtual void loop() override;

public:
    void regNewUI(LVGLUIAPI *api);

    template <typename T>
    inline void regNewUI()
    {
        this->regNewUI(new T());
    }
};

#endif