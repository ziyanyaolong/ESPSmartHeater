#ifndef ZL_LVGL_MANAGER_H
#define ZL_LVGL_MANAGER_H

#include <lvgl.h>
#include <list>

#include "Peripheral/LVGLDriver/LVGLDriver.h"

#include "Software/LVGLManager/API/LVGLUIAPI.h"
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
#ifndef ZLHS_SIMULATOR_DEBUG
    LVGLDriver* lvglDriver = nullptr;
#endif

    LVGLUIAPI::TimeType newTime = 0;

    std::list<LVGLUIAPI *> uiList;

    lv_obj_t *menu;
    lv_obj_t *menuSub;
    lv_obj_t *menuWidget;
    lv_obj_t *section;

private:
    lv_obj_t *createNewPage(lv_obj_t *parent, lv_obj_t *uiPage, const std::string &name);

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