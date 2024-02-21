#ifndef ZL_TEMPERATURE_CONTROL_H
#define ZL_TEMPERATURE_CONTROL_H

#include <PID_v1.h>

#include "CodeGenerate/CodeGenerate.h"

#include "Peripheral/Heater/Heater.h"
#include "Peripheral/ZL_CS1238/ZL_CS1238.h"
#include "Peripheral/ZL_INA219/ZL_INA219.h"

#include "API/SoftwareAPI.h"
#include "ZLHSSystem/ZLHSSystem.h"
#include "CustomMath/CustomMath.h"
#include "IPC/MemoryShader/MemoryShader.h"

class TemperatureControl : public SoftwareAPI
{
public:
    TemperatureControl();
    virtual ~TemperatureControl();

private:
    typedef struct DynamicValue
    {
        DynamicValue(double real = 0.0f,
                     double base = 0.0f,
                     double min = 0.0f,
                     double max = 0.0f)
            : realValue(real),
              baseValue(base),
              minValue(min),
              maxValue(max) {}
        double realValue = 0.0f;
        double baseValue = 0.0f;
        double minValue = 0.0f;
        double maxValue = 0.0f;
    } DynamicValue;

private:
    PID *pid;

    Heater *heater;
    ZL_CS1238 *zl_CS1238;
    ZL_INA219 *zl_INA219;

    DynamicValue pValue = DynamicValue(0.4f, 0.4f, 0.2f, 0.6f);
    DynamicValue iValue = DynamicValue(0.5f, 0.5f, 0.45f, 0.55f);
    DynamicValue dValue = DynamicValue(0.0008f, 0.0008f, 0.0008f, 0.0008f);
    DynamicValue temperatureDifference = DynamicValue(0.0f, 0.0f, 2.0f, 20.0f);

    double inValue = 0.0f;
    double outValue = 0.0f;
    double targetValue = 0.0f;

private:
    void dynamicRangeConstraint(double inValue, DynamicValue &outValue);
    void dynamicMap(DynamicValue &outValue);

protected:
    virtual int init() override;
    virtual void loop() override;

public:
    DEFINE_ZLHS_DATA_NO_DE(targetValue, Target, double, public);
};

#endif