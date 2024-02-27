#ifndef ZL_DISPLAY_DRIVER_H
#define ZL_DISPLAY_DRIVER_H

#include <LovyanGFX.hpp>

#include <Arduino.h>
#include <lvgl.h>
#include <Preferences.h>

#include "API/PeripheralAPI.h"

class DisplayDriver : public lgfx::LGFX_Device, public PeripheralAPI
{
public:
    DisplayDriver();
    virtual ~DisplayDriver();

private:
    Preferences preferences;

    // lgfx::Panel_ILI9163 panelInstance;
    lgfx::Panel_ILI9341 panelInstance;
    // lgfx::Panel_ILI9342 panelInstance;
    // lgfx::Panel_ILI9481 panelInstance;
    // lgfx::Panel_ILI9486 panelInstance;
    // lgfx::Panel_ILI9488 panelInstance;

    lgfx::Bus_SPI busInstance;

    lgfx::Light_PWM lightInstance;

    lgfx::Touch_XPT2046 touchInstance;

    spi_host_device_t tftSPIDevice = spi_host_device_t::SPI2_HOST;
    spi_host_device_t tasSPIDevice = spi_host_device_t::SPI3_HOST;

private:
    void screenInit();
    void touchInit();
    void blInit();

protected:
    virtual int load() override;
    virtual int unload() override;

public:
    void touchXYMapUpdata(uint16_t xMin, uint16_t xMax, uint16_t yMin, uint16_t yMax);

public:
    bool setDefaultTouchMap();

    // 快速生成Set和Gat函数
    DEFINE_ZLHS_DATA_NO_DE(tftSPIDevice, TFTSPIDevice, spi_host_device_t, public);
    DEFINE_ZLHS_DATA_NO_DE(tasSPIDevice, TASSPIDevice, spi_host_device_t, public);
};

#endif