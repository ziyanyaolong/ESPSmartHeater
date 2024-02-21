#ifndef ZL_ENCODER_DRIVER_H
#define ZL_ENCODER_DRIVER_H

#include <Arduino.h>
#include <ESP32Encoder.h>

#include "Peripheral/ZL_Buzzer/ZL_Buzzer.h"

#include "API/PeripheralAPI.h"
#include "CustomMath/CustomMath.h"

#include "ZLHSSystem/ZLHSSystem.h"

class EncoderDriver : public PeripheralAPI
{
public:
    EncoderDriver();
    virtual ~EncoderDriver();

private:
    ZL_Buzzer *zl_Buzzer = nullptr;

    ZLTypes::PINType swPin = -1;
    ZLTypes::PINType sckPin = -1;
    ZLTypes::PINType dtPin = -1;

    int64_t count = 0;

    bool buzzerActivate = true;

    bool pushNow = false;

private:
    static void countEvent(void* classPointer);
    static void buttonEvent(void* classPointer);

protected:
    virtual int load() override;
    virtual int unload() override;

public:
    bool getPushNow(bool flush = true);
    int64_t getCount(bool flush = true);

};

#endif