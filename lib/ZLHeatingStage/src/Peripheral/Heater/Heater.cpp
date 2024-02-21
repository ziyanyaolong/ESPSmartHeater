#include "Heater.h"

Heater::Heater()
    : PeripheralAPI(ZLHS_EX_TO_STRING(Heater))
{
}

Heater::~Heater()
{
}

int Heater::load()
{
    this->pinCon = ZLHS_HEATER_PIN;
    pinMode(this->pinCon, OUTPUT);

    bitNumValue = pow(2, bitNum);

    ledcSetup(channel, freq, bitNum);
    ledcAttachPin(pinCon, channel);

    return 0;
}

int Heater::unload()
{
    return 0;
}

void Heater::setPin(ZLTypes::PINType pin)
{
    this->pinCon = pin;
    pinMode(this->pinCon, OUTPUT);
}

void Heater::setPercentage(double value)
{
    if (value > maxPercentage)
    {
        percentageValue = maxPercentage;
    }
    else if (value < minPercentage)
    {
        percentageValue = minPercentage;
    }
    else
    {
        percentageValue = value;
    }

    ledcWrite(channel, static_cast<uint32_t>(CustomMath::doubleMap(percentageValue, minPercentage, maxPercentage, 0, bitNumValue)));
}

double Heater::getPercentage()
{
    return this->percentageValue;
}

void Heater::setPercentageMin(double value)
{
    this->minPercentage = value;
}

double Heater::getPercentageMin()
{
    return this->minPercentage;
}

void Heater::setPercentageMax(double value)
{
    this->maxPercentage = value;
}

double Heater::getPercentageMax()
{
    return this->maxPercentage;
}
