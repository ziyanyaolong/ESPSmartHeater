#ifndef ZL_BUZZER_H
#define ZL_BUZZER_H

#include <Arduino.h>
#include "API/PeripheralAPI.h"

#include "CustomMath/PercentageMap/PercentageMap.h"

class ZL_Buzzer : public PeripheralAPI
{
public:
    ZL_Buzzer();
    virtual ~ZL_Buzzer();

private:
    ZLTypes::PINType pin = -1;
    TimerHandle_t timer = nullptr;
    uint32_t waitMs = 30;
    uint8_t channel = 4;

    PercentageMap<uint32_t> perMap;

private:
    static void buzzerTimer(TimerHandle_t xTimer);

protected:
    virtual int load() override;
    virtual int unload() override;

public:
    void buzzerOnce(uint32_t ms = 30, uint32_t maxTimerDelay = portMAX_DELAY);
    void buzzerOnceFromISR(uint32_t ms = 30);
};

#endif