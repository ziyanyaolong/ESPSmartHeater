#include "LVGLDriver.h"

LVGLDriver::LVGLDriver()
    : PeripheralAPI(ZLHS_EX_TO_STRING(LVGLDriver))
{
    this->setTCStackDepth(16384);
}

LVGLDriver::~LVGLDriver()
{
}

void LVGLDriver::dispFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    LVGLDriver *dispPointer = static_cast<LVGLDriver *>(disp->user_data);

    while (dispPointer->driver->dmaBusy())
    {
        dispPointer->driver->waitDMA();
    }

    dispPointer->driver->flush();

    dispPointer->driver->pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::swap565_t *)&color_p->full);

    lv_disp_flush_ready(disp);
}

void LVGLDriver::touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    LVGLDriver *dispPointer = static_cast<LVGLDriver *>(indev_driver->user_data);

    if (dispPointer->tcData.touchCalibration)
    {
        dispPointer->touchCalibrationTest();

        data->state = LV_INDEV_STATE_REL;
        data->point.x = 0;
        data->point.y = 0;
        return;
    }

    data->state = LV_INDEV_STATE_REL;

    if (dispPointer->driver->getTouch<lv_coord_t>(&(data->point.x), &(data->point.y)))
    {
        ZLHS_DEBUG_PRINTF("x:%d, y:%d\r\n", data->point.x, data->point.y);

        data->state = LV_INDEV_STATE_PR;
    }
}

void LVGLDriver::touchCalibrationTest()
{
    lv_indev_data_t data;

    data.state = LV_INDEV_STATE_REL;

    if (this->driver->getTouch<lv_coord_t>(&(data.point.x), &(data.point.y)))
    {
        data.state = LV_INDEV_STATE_PR;
    }

    switch (this->tcData.tcState)
    {
    case TouchCalibrationState::Close:
    {
        this->tcData.tcState = TouchCalibrationState::Start;
        break;
    }

    case TouchCalibrationState::Start:
    {
        this->tcData.tcState = TouchCalibrationState::Point1;
        break;
    }

    case TouchCalibrationState::Point1:
    {
        if (data.state == LV_INDEV_STATE_PR)
        {
            this->tcData.lastState = LV_INDEV_STATE_PR;
            this->tcData.p1.x = data.point.x;
            this->tcData.p1.y = data.point.y;
        }
        else if ((this->tcData.lastState != LV_INDEV_STATE_REL) && (data.state == LV_INDEV_STATE_REL))
        {
            ZLHS_DEBUG_PRINTF("x1:%d, y1:%d\r\n", this->tcData.p1.x, this->tcData.p1.y);

            this->tcData.lastState = LV_INDEV_STATE_REL;
            this->tcData.tcState = TouchCalibrationState::Point2;
        }
        break;
    }

    case TouchCalibrationState::Point2:
    {
        if (data.state == LV_INDEV_STATE_PR)
        {
            this->tcData.lastState = LV_INDEV_STATE_PR;
            this->tcData.p2.x = data.point.x;
            this->tcData.p2.y = data.point.y;
        }
        else if ((this->tcData.lastState != LV_INDEV_STATE_REL) && (data.state == LV_INDEV_STATE_REL))
        {
            ZLHS_DEBUG_PRINTF("x2:%d, y2:%d\r\n", this->tcData.p2.x, this->tcData.p2.y);

            this->tcData.lastState = LV_INDEV_STATE_REL;
            this->tcData.tcState = TouchCalibrationState::Point3;
        }
        break;
    }

    case TouchCalibrationState::Point3:
    {
        if (data.state == LV_INDEV_STATE_PR)
        {
            this->tcData.lastState = LV_INDEV_STATE_PR;
            this->tcData.p3.x = data.point.x;
            this->tcData.p3.y = data.point.y;
        }
        else if ((this->tcData.lastState != LV_INDEV_STATE_REL) && (data.state == LV_INDEV_STATE_REL))
        {
            ZLHS_DEBUG_PRINTF("x3:%d, y3:%d\r\n", this->tcData.p3.x, this->tcData.p3.y);

            this->tcData.lastState = LV_INDEV_STATE_REL;
            this->tcData.tcState = TouchCalibrationState::Point4;
        }
        break;
    }

    case TouchCalibrationState::Point4:
    {
        if (data.state == LV_INDEV_STATE_PR)
        {
            this->tcData.lastState = LV_INDEV_STATE_PR;
            this->tcData.p4.x = data.point.x;
            this->tcData.p4.y = data.point.y;
        }
        else if ((this->tcData.lastState != LV_INDEV_STATE_REL) && (data.state == LV_INDEV_STATE_REL))
        {
            ZLHS_DEBUG_PRINTF("x4:%d, y4:%d\r\n", this->tcData.p4.x, this->tcData.p4.y);

            this->tcData.xMin = static_cast<lv_coord_t>((((double)(this->tcData.p1.x)) + ((double)(this->tcData.p2.x))) / ((double)(2.0f)));
            this->tcData.xMax = static_cast<lv_coord_t>((((double)(this->tcData.p3.x)) + ((double)(this->tcData.p4.x))) / ((double)(2.0f)));

            this->tcData.yMin = static_cast<lv_coord_t>((((double)(this->tcData.p2.y)) + ((double)(this->tcData.p4.y))) / ((double)(2.0f)));
            this->tcData.yMax = static_cast<lv_coord_t>((((double)(this->tcData.p1.y)) + ((double)(this->tcData.p3.y))) / ((double)(2.0f)));

            ZLHS_DEBUG_PRINTF("xMin:%d, xMax:%d, yMin:%d, yMax:%d\r\n", this->tcData.xMin, this->tcData.xMax, this->tcData.yMin, this->tcData.yMax);

            this->driver->touchXYMapUpdata((uint16_t)(this->tcData.xMin), (uint16_t)(this->tcData.xMax), (uint16_t)(this->tcData.yMin), (uint16_t)(this->tcData.yMax));
            this->tcData.tcState = TouchCalibrationState::Close;
            this->tcData.touchCalibration = false;
        }
        break;
    }

    default:
        break;
    }
}

int LVGLDriver::load()
{
    driver = ZLHSSystem::getRegAPICast<DisplayDriver>(ZLHS_EX_TO_STRING(DisplayDriver), BaseSystemAPI::APIType::Peripheral);

    if (driver == nullptr)
    {
        return 1;
    }

    lv_init();

    if (lvgl_displayDriver == nullptr)
        lvgl_displayDriver = new LVGL_DisplayDriver;
    if (lvgl_displayData == nullptr)
        lvgl_displayData = new LVGL_DisplayData;

    size_t cacheSize = sizeof(lv_color_t) * lvgl_displayData->cache;

    if (lvgl_displayDriver->buf_1 == nullptr)
    {
        if (enableOutsideRAM)
        {
            lvgl_displayDriver->buf_1 = (lv_color_t *)malloc(cacheSize);
        }
        else
        {
            lvgl_displayDriver->buf_1 = (lv_color_t *)malloc(cacheSize);
        }
    }

    memset(lvgl_displayDriver->buf_1, 0, cacheSize);

    switch (lvgl_displayData->cacheMode)
    {
    case CacheMode::TWO_CACHE:
    {
        if (lvgl_displayDriver->buf_2 == nullptr)
        {
            if (enableOutsideRAM)
            {
                lvgl_displayDriver->buf_2 = (lv_color_t *)malloc(cacheSize);
            }
            else
            {
                lvgl_displayDriver->buf_2 = (lv_color_t *)malloc(cacheSize);
            }
        }

        memset(lvgl_displayDriver->buf_2, 0, cacheSize);
        break;
    }

    default:
        break;
    }

    if (lvgl_displayDriver->disp_buf == nullptr)
    {
        lvgl_displayDriver->disp_buf = new lv_disp_draw_buf_t;
    }

    if (lvgl_displayDriver->disp_drv == nullptr)
    {
        lvgl_displayDriver->disp_drv = new lv_disp_drv_t;
    }

    lv_disp_draw_buf_init(lvgl_displayDriver->disp_buf, lvgl_displayDriver->buf_1, lvgl_displayDriver->buf_2, lvgl_displayData->cache);
    lv_disp_drv_init(lvgl_displayDriver->disp_drv);
    lvgl_displayDriver->disp_drv->flush_cb = &LVGLDriver::dispFlush;
    // lvgl_displayDriver.disp_drv->rounder_cb = &FLSYSTEM::LVGL8_Driver::_rounderFun_;
    // lvgl_displayDriver.disp_drv->set_px_cb = &FLSYSTEM::LVGL8_Driver::_set_pxFun_;
    // lvgl_displayDriver.disp_drv->clear_cb = &FLSYSTEM::LVGL8_Driver::_clearFun_;
    // lvgl_displayDriver.disp_drv->monitor_cb = &FLSYSTEM::LVGL8_Driver::_monitorFun_;
    // lvgl_displayDriver.disp_drv->wait_cb = &FLSYSTEM::LVGL8_Driver::_waitFun_;
    // lvgl_displayDriver.disp_drv->clean_dcache_cb = &FLSYSTEM::LVGL8_Driver::_clean_dcacheFun_;
    // lvgl_displayDriver.disp_drv->drv_update_cb = &FLSYSTEM::LVGL8_Driver::_drv_updateFun_;
    lvgl_displayDriver->disp_drv->draw_buf = lvgl_displayDriver->disp_buf;
    lvgl_displayDriver->disp_drv->direct_mode = lvgl_displayData->directMode;

    lvgl_displayDriver->disp_drv->full_refresh = lvgl_displayData->fullRefresh;
    lvgl_displayDriver->disp_drv->sw_rotate = lvgl_displayData->swRotate;
    lvgl_displayDriver->disp_drv->hor_res = driver->width();
    lvgl_displayDriver->disp_drv->ver_res = driver->height();
    lvgl_displayDriver->disp_drv->dpi = lvgl_displayData->dpi;
    lvgl_displayDriver->disp_drv->rotated = lvgl_displayData->rotated;
    lvgl_displayDriver->disp_drv->antialiasing = lvgl_displayData->antialiasing;
    lvgl_displayDriver->disp_drv->screen_transp = lvgl_displayData->screenTransp;
    lvgl_displayDriver->disp_drv->user_data = static_cast<void *>(this);
    lvgl_displayDriver->disp = lv_disp_drv_register(lvgl_displayDriver->disp_drv);

    if (indevDrv == nullptr)
    {
        indevDrv = new lv_indev_drv_t;
    }

    lv_indev_drv_init(indevDrv);
    indevDrv->type = LV_INDEV_TYPE_POINTER;
    indevDrv->read_cb = &LVGLDriver::touchpadRead;
    indevDrv->user_data = static_cast<void *>(this);
    lv_indev_drv_register(indevDrv);

    return 0;
}

int LVGLDriver::unload()
{
    return 0;
}

void LVGLDriver::touchCalibrationStart()
{
    this->tcData.tcState = TouchCalibrationState::Start;
    this->tcData.touchCalibration = true;
}

LVGLDriver::TouchCalibrationState LVGLDriver::touchCalibrationState()
{
    return this->tcData.tcState;
}
