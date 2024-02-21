#include "ZL_INA219.h"

ZL_INA219::ZL_INA219()
    : PeripheralAPI(ZLHS_EX_TO_STRING(ZL_INA219))
{
}

ZL_INA219::~ZL_INA219()
{
}

int ZL_INA219::load()
{
    if (ina219 == nullptr)
    {
        ina219 = new INA219_WE();
    }

    Wire.setPins(ZLHS_I2C_SDA_PIN, ZLHS_I2C_SCL_PIN);
    Wire.begin();

    if (!(ina219->init()))
    {
        return 1;
    }

    ina219->setADCMode(SAMPLE_MODE_128);
    ina219->setMeasureMode(CONTINUOUS);
    ina219->setPGain(PG_160);
    ina219->setBusRange(BRNG_32);
    ina219->setShuntSizeInOhms(0.02f);
    ina219->setCorrectionFactor(0.98f);
    // ina219->setShuntVoltOffset_mV(0.0f);

    return 0;
}

int ZL_INA219::unload()
{
    return 0;
}

void ZL_INA219::defaultDebugPrint()
{
    float shuntVoltage_mV = 0.0;
    float loadVoltage_V = 0.0;
    float busVoltage_V = 0.0;
    float current_mA = 0.0;
    float power_mW = 0.0;
    bool ina219_overflow = false;

    shuntVoltage_mV = ina219->getShuntVoltage_mV();
    busVoltage_V = ina219->getBusVoltage_V();
    current_mA = ina219->getCurrent_mA() * 10.0f;
    power_mW = ina219->getBusPower() * 10.0f;
    loadVoltage_V = busVoltage_V + (shuntVoltage_mV / 1000.0f);
    ina219_overflow = ina219->getOverflow();

    ZLHS_DEBUG_PRINT("Shunt Voltage [mV]: ");
    ZLHS_DEBUG_PRINTLN(shuntVoltage_mV);
    ZLHS_DEBUG_PRINT("Bus Voltage [V]: ");
    ZLHS_DEBUG_PRINTLN(busVoltage_V);
    ZLHS_DEBUG_PRINT("Load Voltage [V]: ");
    ZLHS_DEBUG_PRINTLN(loadVoltage_V);
    ZLHS_DEBUG_PRINT("Current[A]: ");
    ZLHS_DEBUG_PRINTLN(current_mA / 1000.0f);
    ZLHS_DEBUG_PRINT("Bus Power [W]: ");
    ZLHS_DEBUG_PRINTLN(power_mW / 1000.0f);

    if (!ina219_overflow)
    {
        ZLHS_DEBUG_PRINTLN("Values OK - no overflow");
    }
    else
    {
        ZLHS_DEBUG_PRINTLN("Overflow! Choose higher PGAIN");
    }

    ZLHS_DEBUG_PRINTLN();
}

float ZL_INA219::getVoltageV()
{
    float shuntVoltage_mV = 0.0;
    float loadVoltage_V = 0.0;
    float busVoltage_V = 0.0;

    shuntVoltage_mV = ina219->getShuntVoltage_mV();
    busVoltage_V = ina219->getBusVoltage_V();
    loadVoltage_V = busVoltage_V + (shuntVoltage_mV / 1000.0f);
    return loadVoltage_V;
}

float ZL_INA219::getCurrentA()
{
    return this->ina219->getCurrent_mA() / 100.0f;
}