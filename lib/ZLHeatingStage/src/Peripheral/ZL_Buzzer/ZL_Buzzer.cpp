#include "ZL_Buzzer.h"

ZL_Buzzer::ZL_Buzzer()
    : PeripheralAPI(ZLHS_EX_TO_STRING(ZL_Buzzer))
{
}

ZL_Buzzer::~ZL_Buzzer()
{
}

void ZL_Buzzer::buzzerTimer(TimerHandle_t xTimer)
{
    ZL_Buzzer *pointer = static_cast<ZL_Buzzer *>(pvTimerGetTimerID(xTimer));

    if (pointer == nullptr)
    {
        return;
    }

    ledcWrite(pointer->channel, 0);
}

int ZL_Buzzer::load()
{
    this->pin = ZLHS_BUZZER_PIN;

    ledcSetup(this->channel, 2000, 12);
    ledcAttachPin(this->pin, this->channel);
    ledcWrite(this->channel, 0);

    perMap.setMapData(0.0f, 100.0f, 0, pow(2, 12));

    perMap.setMapValue(50.0f);

    timer = xTimerCreate("BuzzerTimer",
                         pdMS_TO_TICKS(this->waitMs),
                         pdFALSE,
                         static_cast<void *>(this),
                         &ZL_Buzzer::buzzerTimer);

    xTimerStop(timer, portMAX_DELAY);

    return 0;
}

int ZL_Buzzer::unload()
{
    if (timer)
    {
        xTimerDelete(timer, portMAX_DELAY);
    }

    return 0;
}

void ZL_Buzzer::buzzerOnce(uint32_t ms, uint32_t maxTimerDelay)
{
    if (this->waitMs != ms)
    {
        this->waitMs = ms;
        xTimerChangePeriod(timer, pdMS_TO_TICKS(this->waitMs), pdMS_TO_TICKS(maxTimerDelay));
    }

    if (xTimerIsTimerActive(timer) == pdFALSE)
    {
        ledcWrite(this->channel, this->perMap.getMapValue());

        if (xTimerReset(timer, pdMS_TO_TICKS(maxTimerDelay)) == pdFAIL)
        {
            ledcWrite(this->channel, 0);
        }
    }
}

void ZL_Buzzer::buzzerOnceFromISR(uint32_t ms)
{
    if (this->waitMs != ms)
    {
        this->waitMs = ms;
        xTimerChangePeriodFromISR(timer, pdMS_TO_TICKS(this->waitMs), nullptr);
    }

    if (xTimerIsTimerActive(timer) == pdFALSE)
    {
        ledcWrite(this->channel, this->perMap.getMapValue());

        if (xTimerResetFromISR(timer, nullptr) == pdFAIL)
        {
            ledcWrite(this->channel, 0);
        }
    }
}
