#ifndef ZL_HEATER_H
#define ZL_HEATER_H

#include <Arduino.h>
#include "CustomMath/CustomMath.h"
#include "CodeGenerate/CodeGenerate.h"
#include "API/PeripheralAPI.h"

class Heater : public PeripheralAPI
{
private:
    uint8_t channel = 0;
    uint32_t freq = 1500;
    uint8_t bitNum = 12;
    double bitNumValue = 0.0f;
    ZLTypes::PINType pinCon = -1;
    double maxPercentage = 80;
    double minPercentage = 0;
    double percentageValue = minPercentage;

protected:
    virtual int load() override;
    virtual int unload() override;

public:
    Heater();
    virtual ~Heater();

    void setPin(ZLTypes::PINType pin);

    void setPercentage(double value);
    double getPercentage();

    void setPercentageMin(double value);
    double getPercentageMin();

    void setPercentageMax(double value);
    double getPercentageMax();

};

#endif