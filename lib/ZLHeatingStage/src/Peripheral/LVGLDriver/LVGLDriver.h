#ifndef ZL_LVGL_DRIVER_H
#define ZL_LVGL_DRIVER_H

#include <lvgl.h>

#include "Peripheral/DisplayDriver/DisplayDriver.h"
#include "ZLHSSystem/ZLHSSystem.h"

#include "CodeGenerate/CodeGenerate.h"
#include "API/PeripheralAPI.h"

class LVGLDriver : public PeripheralAPI
{
public:
    LVGLDriver();
    virtual ~LVGLDriver();

    enum class CacheMode
    {
        ONE_CACHE = 0X00,
        TWO_CACHE = 0X01
    };

    enum class TouchCalibrationState
    {
        Close = 0X00,
        Start,
        Point1,
        Point2,
        Point3,
        Point4
    };

    typedef struct LVGL_DisplayData
    {
        bool directMode = false;
        uint8_t swRotate = 0;
        uint8_t rotated = 0;
        bool fullRefresh = 0;
        bool antialiasing = 0;
        bool screenTransp = 0;
        uint32_t dpi = 14;
        size_t cache = 10240;
        CacheMode cacheMode = CacheMode::TWO_CACHE;
    } LVGL_DisplayData;

    typedef struct LVGL_DisplayDriver
    {
        lv_disp_draw_buf_t *disp_buf = nullptr;
        lv_disp_drv_t *disp_drv = nullptr;
        lv_disp_t *disp = nullptr;
        lv_color_t *buf_1 = nullptr;
        lv_color_t *buf_2 = nullptr;
    } LVGL_DisplayDriver;

    typedef struct TouchCalibrationData
    {
        // 辅助参数
        bool touchCalibration = false;
        lv_indev_state_t lastState = LV_INDEV_STATE_REL;
        TouchCalibrationState tcState = TouchCalibrationState::Close;

        // 数据点
        lv_point_t p1;
        lv_point_t p2;
        lv_point_t p3;
        lv_point_t p4;

        // 映射数据
        lv_coord_t xMin = 0;
        lv_coord_t xMax = 0;
        lv_coord_t yMin = 0;
        lv_coord_t yMax = 0;
    } TouchCalibrationData;

private:
    TouchCalibrationData tcData;

    DisplayDriver *driver = nullptr;
    LVGL_DisplayDriver *lvgl_displayDriver = nullptr;
    LVGL_DisplayData *lvgl_displayData = nullptr;
    lv_indev_drv_t *indevDrv = nullptr;

    bool enableOutsideRAM = false;

private:
    static void dispFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    static void touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
    void touchCalibrationTest();

protected:
    virtual int load() override;
    virtual int unload() override;

public:
    void touchCalibrationStart();
    TouchCalibrationState touchCalibrationState();
};

#endif