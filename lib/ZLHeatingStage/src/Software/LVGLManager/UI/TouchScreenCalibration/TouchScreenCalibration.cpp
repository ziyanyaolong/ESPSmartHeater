#include "TouchScreenCalibration.h"

TouchScreenCalibration::TouchScreenCalibration()
{
}

TouchScreenCalibration::~TouchScreenCalibration()
{
}

void TouchScreenCalibration::touchCStart(lv_event_t *e)
{
    if ((lv_event_get_code(e) != LV_EVENT_PRESSED) || (lv_event_get_param(e) == nullptr))
    {
        return;
    }

    auto pointer = static_cast<TouchScreenCalibration *>(lv_event_get_param(e));

    if (pointer == nullptr)
    {
        return;
    }

    lv_obj_add_flag(lv_event_get_current_target(e), LV_OBJ_FLAG_HIDDEN);

    pointer->driver->touchCalibrationStart();
}

uint8_t TouchScreenCalibration::init()
{
    driver = ZLHSSystem::getRegAPICast<LVGLDriver>(ZLHS_EX_TO_STRING(LVGLDriver), BaseSystemAPI::APIType::Peripheral);

    if (driver == nullptr)
    {
        return -1;
    }
}

void TouchScreenCalibration::setupUI(lv_obj_t *page)
{
    lv_style_reset(&(this->styleBtnPA));                                               // 重置按钮样式
    lv_style_init(&(this->styleBtnPA));                                                // 初始化按钮样式
    lv_style_set_radius(&(this->styleBtnPA), 3);                                       // 设置按钮样式圆角弧度
    lv_style_set_bg_color(&(this->styleBtnPA), lv_palette_lighten(LV_PALETTE_RED, 3)); // 设置背景的颜色为淡灰色
    lv_style_set_border_color(&(this->styleBtnPA), lv_color_white());                  // 设置边框颜色为黑色
    lv_style_set_border_width(&(this->styleBtnPA), 2);                                 // 设置边框宽度为2
    lv_style_set_text_color(&(this->styleBtnPA), lv_color_black());                    // 设置字体颜色为黑色

    lv_style_reset(&(this->styleBtnPB));                                                // 重置按钮样式
    lv_style_init(&(this->styleBtnPB));                                                 // 初始化按钮样式
    lv_style_set_radius(&(this->styleBtnPB), 3);                                        // 设置按钮样式圆角弧度
    lv_style_set_bg_color(&(this->styleBtnPB), lv_palette_lighten(LV_PALETTE_BLUE, 3)); // 设置背景的颜色为淡灰色
    lv_style_set_border_color(&(this->styleBtnPB), lv_color_white());                   // 设置边框颜色为黑色
    lv_style_set_border_width(&(this->styleBtnPB), 2);                                  // 设置边框宽度为2
    lv_style_set_text_color(&(this->styleBtnPB), lv_color_black());                     // 设置字体颜色为黑色

    this->btnStart = lv_btn_create(page);
    lv_obj_set_size(this->btnStart, 100, 50);
    lv_obj_align(this->btnStart, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(this->btnStart, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(this->btnStart, &TouchScreenCalibration::touchCStart, LV_EVENT_PRESSED, static_cast<void *>(this));

    this->btnP1 = lv_btn_create(page);
    lv_obj_set_size(this->btnP1, this->btnPSize, this->btnPSize);
    lv_obj_align(this->btnP1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(this->btnP1, &(this->styleBtnPA), 0);

    this->btnP2 = lv_btn_create(page);
    lv_obj_set_size(this->btnP2, this->btnPSize, this->btnPSize);
    lv_obj_align(this->btnP2, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_style(this->btnP2, &(this->styleBtnPA), 0);

    this->btnP3 = lv_btn_create(page);
    lv_obj_set_size(this->btnP3, this->btnPSize, this->btnPSize);
    lv_obj_align(this->btnP3, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_style(this->btnP3, &(this->styleBtnPA), 0);

    this->btnP4 = lv_btn_create(page);
    lv_obj_set_size(this->btnP4, this->btnPSize, this->btnPSize);
    lv_obj_align(this->btnP4, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_style(this->btnP4, &(this->styleBtnPA), 0);

    this->driver->touchCalibrationStart();
    lv_obj_add_flag(this->btnStart, LV_OBJ_FLAG_HIDDEN);

    this->labelECount = lv_label_create(page);
    lv_obj_set_size(this->labelECount, 200, 100);
    lv_obj_align(this->labelECount, LV_ALIGN_CENTER, 0, 0);
}

void TouchScreenCalibration::loop()
{
    tcStateTemp = this->driver->touchCalibrationState();
    switch (tcStateTemp)
    {
    case LVGLDriver::TouchCalibrationState::Close:
    {
        if (tcState != tcStateTemp)
        {
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP4, &(this->styleBtnPA), 0);
            lv_obj_clear_flag(this->btnStart, LV_OBJ_FLAG_HIDDEN);
            tcState = tcStateTemp;
            ZLHS_DEBUG_PRINTLN("Close");
        }
        break;
    }

    case LVGLDriver::TouchCalibrationState::Point1:
    {
        if (tcState != tcStateTemp)
        {
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP1, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP4, &(this->styleBtnPA), 0);
            tcState = tcStateTemp;
            ZLHS_DEBUG_PRINTLN("P1");
        }
        break;
    }

    case LVGLDriver::TouchCalibrationState::Point2:
    {
        if (tcState != tcStateTemp)
        {
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP2, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP4, &(this->styleBtnPA), 0);
            tcState = tcStateTemp;
            ZLHS_DEBUG_PRINTLN("P2");
        }
        break;
    }

    case LVGLDriver::TouchCalibrationState::Point3:
    {
        if (tcState != tcStateTemp)
        {
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP3, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP4, &(this->styleBtnPA), 0);
            tcState = tcStateTemp;
            ZLHS_DEBUG_PRINTLN("P3");
        }
        break;
    }

    case LVGLDriver::TouchCalibrationState::Point4:
    {
        if (tcState != tcStateTemp)
        {
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPA), 0);
            lv_obj_remove_style(this->btnP1, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP2, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP3, &(this->styleBtnPB), 0);
            lv_obj_remove_style(this->btnP4, &(this->styleBtnPB), 0);
            lv_obj_add_style(this->btnP1, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP2, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP3, &(this->styleBtnPA), 0);
            lv_obj_add_style(this->btnP4, &(this->styleBtnPB), 0);
            tcState = tcStateTemp;
            ZLHS_DEBUG_PRINTLN("P4");
        }
        break;
    }

    default:
        break;
    }
    MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<void *>(&(this->lcValue)));
    lv_label_set_text(this->labelECount, String(this->lcValue, 1).c_str());
}