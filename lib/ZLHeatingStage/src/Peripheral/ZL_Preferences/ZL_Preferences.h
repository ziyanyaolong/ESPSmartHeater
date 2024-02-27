#ifndef ZL_PREFERENCES_H
#define ZL_PREFERENCES_H

#include <Arduino.h>
#include <Preferences.h>

#include "API/PeripheralAPI.h"

class ZL_Preferences : public PeripheralAPI
{
public:
    ZL_Preferences();
    virtual ~ZL_Preferences();

private:
    Preferences preferences;

protected:
    virtual int load() override;
    virtual int unload() override;

public:
};

#endif