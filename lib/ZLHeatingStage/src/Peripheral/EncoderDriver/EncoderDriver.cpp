#include "EncoderDriver.h"

EncoderDriver::EncoderDriver()
    : PeripheralAPI(ZLHS_EX_TO_STRING(EncoderDriver))
{
}

EncoderDriver::~EncoderDriver()
{
}

void ICACHE_RAM_ATTR EncoderDriver::countEvent(void *classPointer)
{
    auto pointer = static_cast<EncoderDriver *>(classPointer);

    if (digitalRead(pointer->sckPin))
    {
        pointer->count++;
    }
    else
    {
        pointer->count--;
    }

    if (pointer->buzzerActivate)
    {
        pointer->zl_Buzzer->buzzerOnceFromISR();
    }
}

void ICACHE_RAM_ATTR EncoderDriver::buttonEvent(void *classPointer)
{
    auto pointer = static_cast<EncoderDriver *>(classPointer);

    pointer->pushNow = !digitalRead(pointer->swPin);

    if (pointer->buzzerActivate)
    {
        pointer->zl_Buzzer->buzzerOnceFromISR();
    }
}

int EncoderDriver::load()
{
    this->swPin = ZLHS_EC11_SW_PIN;
    this->sckPin = ZLHS_EC11_SCK_PIN;
    this->dtPin = ZLHS_EC11_DT_PIN;

    pinMode(this->sckPin, INPUT);
    pinMode(this->dtPin, INPUT);

    this->zl_Buzzer = ZLHSSystem::getRegAPICast<ZL_Buzzer>(ZLHS_EX_TO_STRING(ZL_Buzzer), BaseSystemAPI::APIType::Peripheral);

    attachInterruptArg(this->dtPin, &EncoderDriver::countEvent, static_cast<void *>(this), FALLING);
    attachInterruptArg(this->swPin, &EncoderDriver::buttonEvent, static_cast<void *>(this), FALLING);

    this->pushNow = !digitalRead(this->swPin);

    return 0;
}

int EncoderDriver::unload()
{
    detachInterrupt(this->dtPin);
    detachInterrupt(this->swPin);

    return 0;
}

bool EncoderDriver::getPushNow(bool flush)
{
    if (flush)
    {
        auto tValue = this->pushNow;
        this->pushNow = false;
        return tValue;
    }
    else
    {
        return this->pushNow;
    }
}

int64_t EncoderDriver::getCount(bool flush)
{
    if (flush)
    {
        auto tValue = this->count;
        this->count = 0;
        return tValue;
    }
    else
    {
        return this->count;
    }
}
