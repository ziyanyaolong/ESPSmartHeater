#include "LVGLManager.h"

LVGLManager::LVGLManager()
    : SoftwareAPI(ZLHS_EX_TO_STRING(LVGLManager))
{
    this->setTCRunTimeDelay(5);
    this->setTCStackDepth(32768);
    this->setTCPriority(10);
}

LVGLManager::~LVGLManager()
{
}

int LVGLManager::init()
{
    driver = ZLHSSystem::getRegAPICast<LVGLDriver>(ZLHS_EX_TO_STRING(LVGLDriver), BaseSystemAPI::APIType::Peripheral);

    if (driver == nullptr)
    {
        return -1;
    }

    baseDriver = ZLHSSystem::getRegAPICast<DisplayDriver>(ZLHS_EX_TO_STRING(DisplayDriver), BaseSystemAPI::APIType::Peripheral);

    encoderDriver = ZLHSSystem::getRegAPICast<EncoderDriver>(ZLHS_EX_TO_STRING(EncoderDriver), BaseSystemAPI::APIType::Peripheral);

    zl_Buzzer = ZLHSSystem::getRegAPICast<ZL_Buzzer>(ZLHS_EX_TO_STRING(ZL_Buzzer), BaseSystemAPI::APIType::Peripheral);

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_STATE_DEFAULT);

    // this->regNewUI<TouchScreenCalibration>();
    this->regNewUI<MainUI>();

    return 0;
}

void LVGLManager::loop()
{
    lv_task_handler();

    for (auto iter = uiList.begin(); iter != uiList.end(); iter++)
    {
        switch ((*iter)->lState)
        {
        case LVGLUIAPI::APILoadStage::Init:
        {
            ZLHS_DEBUG_PRINTLN("LVGL:UI Init OK");
            (*iter)->init();
            (*iter)->page = lv_scr_act();
            (*iter)->setupUI((*iter)->page);
            (*iter)->lState = LVGLUIAPI::APILoadStage::Loop;
            break;
        }

        case LVGLUIAPI::APILoadStage::Loop:
        {
            (*iter)->loop();
            break;
        }

        default:
            break;
        }
    }

    this->eValue += (double)encoderDriver->getCount();

    MemoryShader::instance()->writeData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<const void *>(&eValue));
}

void LVGLManager::regNewUI(LVGLUIAPI *api)
{
    uiList.push_back(api);
}
