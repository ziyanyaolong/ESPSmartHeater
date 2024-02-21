#ifndef ZL_INA219_H
#define ZL_INA219_H

#include <Arduino.h>
#include <Wire.h>
#include <INA219_WE.h>

#include "API/PeripheralAPI.h"

class ZL_INA219 : public PeripheralAPI
{
public:
    ZL_INA219();
    virtual ~ZL_INA219();

private:
    INA219_WE *ina219 = nullptr;

protected:
    virtual int load() override;
    virtual int unload() override;

public:
    void defaultDebugPrint();
};

#endif