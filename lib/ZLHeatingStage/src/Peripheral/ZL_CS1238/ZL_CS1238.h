#ifndef ZL_CS1238_H
#define ZL_CS1238_H

#include <Arduino.h>
#include "API/PeripheralAPI.h"

#include "CustomMath/CustomMath.h"

#define ZL_CS1238_REG_WRITE_CMD 0xCA
#define ZL_CS1238_REG_READ_CMD 0xAC

class ZL_CS1238 : public PeripheralAPI
{
public:
    enum class SPEEDValue
    {
        Value_10Hz = 0x00,
        Value_40Hz = 0x02,
        Value_640Hz = 0x01,
        Value_1280Hz = 0x03,
        Now = 0xAA
    };

    enum class PGAValue
    {
        Value_1 = 0x00,
        Value_2 = 0x02,
        Value_64 = 0x01,
        Value_128 = 0x03,
        Now = 0xAA
    };

    enum class ADCChannel
    {
        ADC1 = 0x00,
        ADC2 = 0x02,
        Temperature = 0x01,
        ShortCircuit = 0x03,
        Now = 0xAA
    };

    typedef struct RegConfigBits
    {
        uint8_t adcCase : 2;
        uint8_t pgaCase : 2;
        uint8_t adcFre : 2;
        uint8_t refOn : 1;
        uint8_t rev : 1;
    } RegConfigBits;

    typedef union RegConfig
    {
        RegConfigBits bits;
        uint8_t allValue = (uint8_t)0x0;
    } RegConfig;

    typedef struct ADCMapRange
    {
        double minValue = 1.0f;
        double maxValue = 1.0f;
        double minReal = 1.0f;
        double maxReal = 1.0f;
    } ADCMapRange;

    // 当前为内部温度传感器
private:
    bool sleepNow = false;

    ZLTypes::PINType pinSck = -1;
    ZLTypes::PINType pinData = -1;

    int32_t adcValue1 = -1;
    int32_t adcValue2 = -1;
    int32_t temperatureValue = -1;
    int32_t shortCircuitValue = -1;

    RegConfig regConfig;

    float refValue = 5.0f;
    float baseTemperature = 24.878599f;
    float baseTemperatureADCValue = 3875;

    ADCMapRange adcRange1;
    ADCMapRange adcRange2;

    SPEEDValue speedValue = SPEEDValue::Value_10Hz;
    PGAValue pgaValue = PGAValue::Value_128;
    ADCChannel adcChannel = ADCChannel::ADC1;

    void sckClock();
    void caseOut();
    void caseIn();
    void regOp(bool isRead);

protected:
    virtual int load() override;
    virtual int unload() override;

public:
    ZL_CS1238();
    virtual ~ZL_CS1238();

    int32_t readADC(ADCChannel channel = ADCChannel::Now);

    void writeReg();
    RegConfig readReg(bool isReadTemp = false);

    void setREFType(float value);

    void setREFOUT(bool value, bool isUpdataNow = true);
    void setPGAValue(PGAValue value, bool isUpdataNow = true);
    void setSPEEDValue(SPEEDValue value, bool isUpdataNow = true);
    void setADCChannel(ADCChannel channel, bool isUpdataNow = true);

    void temperatureCalibration(float baseTValue);

    double readADCRealValue(bool isUpdata = true);

    void setSleep(bool value);

    void setADCRange(ADCChannel channel, double minValue, double maxValue, double minReal, double maxReal);
};

#endif