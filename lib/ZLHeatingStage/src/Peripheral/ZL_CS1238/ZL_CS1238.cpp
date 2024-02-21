#include "ZL_CS1238.h"

ZL_CS1238::ZL_CS1238()
    : PeripheralAPI(ZLHS_EX_TO_STRING(ZL_CS1238))
{
}

ZL_CS1238::~ZL_CS1238()
{
}

int ZL_CS1238::load()
{
    this->pinSck = ZLHS_CS1238_SCK_PIN;
    this->pinData = ZLHS_CS1238_DATA_PIN;
    pinMode(pinSck, OUTPUT);
    pinMode(pinData, OUTPUT);
    this->setREFOUT(true, false);
    this->setSPEEDValue(SPEEDValue::Value_10Hz, false);
    this->setPGAValue(PGAValue::Value_128, false);
    this->setADCChannel(ADCChannel::ADC1, false);
    this->writeReg();

    return 0;
}

int ZL_CS1238::unload()
{
    return 0;
}

void ZL_CS1238::sckClock()
{
    digitalWrite(pinSck, HIGH);
    delayMicroseconds(4);
    digitalWrite(pinSck, LOW);
    delayMicroseconds(4);
}

void ZL_CS1238::caseOut()
{
    pinMode(pinSck, OUTPUT);
    pinMode(pinData, OUTPUT);
}

void ZL_CS1238::caseIn()
{
    pinMode(pinSck, OUTPUT);
    pinMode(pinData, INPUT);
}

void ICACHE_RAM_ATTR ZL_CS1238::regOp(bool isRead)
{
    int i = 0;
    uint8_t regValue = 0x00;

    // DOUT由高变低之后开始读取数据
    this->caseOut();
    digitalWrite(pinData, HIGH);
    this->caseIn();
    digitalWrite(pinData, LOW);

    // 1-29,读ADC数值
    for (i = 0; i < 29; i++)
    {
        this->sckClock();
    }

    this->caseOut();
    if (isRead)
    {
        regValue = ZL_CS1238_REG_READ_CMD; // 因为只有7个时钟,所以左移一位
    }
    else
    {
        regValue = ZL_CS1238_REG_WRITE_CMD; // 因为只有7个时钟,所以左移一位
    }

    /* 1: clk30 ~ clk36  写入读命令*/
    for (i = 0; i < 7; i++)
    {

        if ((regValue & 0x80) != 0)
        {
            digitalWrite(pinData, HIGH);
        }
        else
        {
            digitalWrite(pinData, LOW);
        }

        regValue = regValue << 1;
        this->sckClock();
    }

    // clk37切换写入写出方向
    this->sckClock();

    if (isRead)
    {
        this->caseIn();
    }
    else
    {
        this->caseOut();
    }

    /* 1: clk38 ~ clk45 输出寄存器配置*/
    if (isRead)
    {
        regValue = 0x00;
    }
    else
    {
        memcpy(&regValue, (uint8_t *)(&this->regConfig.allValue), sizeof(uint8_t));
    }

    for (i = 0; i < 8; i++)
    {
        if (isRead)
        {
            regValue <<= 1;
            this->sckClock();
            if (digitalRead(pinData) == 1)
            {
                regValue |= 0x01; // 为了保证前面输入的数据不变新输入的数据为高电平1时可以传进来
            }
        }
        else
        {
            if (regValue & 0x80)
            {
                digitalWrite(pinData, HIGH);
            }
            else
            {
                digitalWrite(pinData, LOW);
            }
            regValue <<= 1;
            this->sckClock();
        }
    }

    this->caseOut();
    digitalWrite(pinData, HIGH);
    this->sckClock();

    if (isRead)
    {
        this->regConfig.bits.refOn = (regValue) >> (1) & 0xff;
        this->regConfig.bits.adcFre = (regValue) >> (2) & 0xff;
        this->regConfig.bits.pgaCase = (regValue) >> (4) & 0xff;
        this->regConfig.bits.adcCase = (regValue) >> (6) & 0xff;
    }
}

int32_t ICACHE_RAM_ATTR ZL_CS1238::readADC(ADCChannel channel)
{
    uint8_t time = 0;
    uint32_t tempData = 0;
    int32_t tempADCValue = 0;

#ifdef ESP32
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    portENTER_CRITICAL(&mux);
#else
    noInterrupts();
#endif

    this->caseOut();
    digitalWrite(pinData, HIGH);
    this->caseIn();
    digitalWrite(pinData, LOW);

#ifdef ESP32
    portEXIT_CRITICAL(&mux);
#else
    interrupts();
#endif

    while (digitalRead(pinData))
    {
        delayMicroseconds(1);
        time++;
        if (time > 80)
        {
            return -1;
        }
    }

#ifdef ESP32
    portENTER_CRITICAL(&mux);
#else
    noInterrupts();
#endif

    for (int i = 0; i < 24; i++)
    {
        tempData <<= 1;
        this->sckClock();
        if (digitalRead(pinData) == 1)
        {
            tempData |= 0x01;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        this->sckClock();
    }

#ifdef ESP32
    portEXIT_CRITICAL(&mux);
#else
    interrupts();
#endif

    if (tempData & 0x00800000)
    {
        tempADCValue = -(((~tempData) & 0x007FFFFF) + 1);
    }
    else
    {
        tempADCValue = (int32_t)tempData; // 正数的补码就是源码;
    }

    switch (this->adcChannel)
    {
    case ADCChannel::ADC1:
        this->adcValue1 = tempADCValue;
        break;

    case ADCChannel::ADC2:
        this->adcValue2 = tempADCValue;
        break;

    case ADCChannel::Temperature:
        this->temperatureValue = tempADCValue;
        break;

    case ADCChannel::ShortCircuit:
        this->shortCircuitValue = tempADCValue;
        break;

    default:
        tempADCValue = 0;
        break;
    }

    // ZLHS_DEBUG_PRINTF("T1:%d, HEX-T1:%x, T2:%d, HEX-T2:%x\n", tempData, tempData, tempADCValue, tempADCValue);

    return tempADCValue;
}

void ZL_CS1238::writeReg()
{
    this->regOp(false);
}

ZL_CS1238::RegConfig ZL_CS1238::readReg(bool isReadTemp)
{
    if (!isReadTemp)
    {
        this->regOp(true);
    }

    return regConfig;
}

void ZL_CS1238::setREFType(float value)
{
    this->refValue = value;
}

void ZL_CS1238::setREFOUT(bool value, bool isUpdataNow)
{
    // 逻辑与数据手册相反，0为启用
    this->regConfig.bits.refOn = !value;

    if (isUpdataNow)
    {
        this->writeReg();
    }
}

void ZL_CS1238::setSPEEDValue(SPEEDValue value, bool isUpdataNow)
{
    if (value != SPEEDValue::Now)
    {
        this->regConfig.bits.adcFre = (uint8_t)value;
        this->speedValue = value;
    }

    if (isUpdataNow)
    {
        this->writeReg();
    }
}

void ZL_CS1238::setPGAValue(PGAValue value, bool isUpdataNow)
{
    if (value != PGAValue::Now)
    {
        this->regConfig.bits.pgaCase = (uint8_t)value;
        this->pgaValue = value;
    }

    if (isUpdataNow)
    {
        this->writeReg();
    }
}

void ZL_CS1238::setADCChannel(ADCChannel channel, bool isUpdataNow)
{
    if (channel != ADCChannel::Now)
    {
        this->regConfig.bits.adcCase = (uint8_t)channel;
        this->adcChannel = channel;
        if (channel == ADCChannel::Temperature)
        {
            this->pgaValue = PGAValue::Value_1;
        }
    }

    if (isUpdataNow)
    {
        this->writeReg();
    }
}

void ZL_CS1238::temperatureCalibration(float baseTValue)
{
    auto tempRegConfig = this->regConfig;

    this->baseTemperature = baseTValue;

    this->setADCChannel(ADCChannel::Temperature, true);

    uint64_t tempValue = 0;

    uint32_t count = 0;

    int32_t tempADCValue = 0;

    vTaskDelay(120);

    for (int i = 0; i < 20; i++)
    {
        tempADCValue = (uint64_t)this->readADC();
        if (tempADCValue > 0)
        {
            tempValue += (uint64_t)tempADCValue;
            count++;
        }
        vTaskDelay(120);
    }

    this->baseTemperatureADCValue = (float)((double)(tempValue) / (double)(count));

    // Serial.printf("TC:%ld, %.4f, %.4f\n", tempValue, this->baseTemperatureADCValue, baseTValue);

    this->regConfig = tempRegConfig;

    this->writeReg();
}

double ZL_CS1238::readADCRealValue(bool isUpdata)
{
    if (isUpdata)
    {
        this->readADC();
    }

    switch (this->adcChannel)
    {
    case ADCChannel::ADC1:
    {
        return CustomMath::doubleMap(((double)(this->adcValue1) / (8388607.0f)) * this->refValue,
                                     this->adcRange1.minValue,
                                     this->adcRange1.maxValue,
                                     this->adcRange1.minReal,
                                     this->adcRange1.maxReal);
        // return ((double)(this->adcValue1) / (8388607.0f)) * this->refValue;
        break;
    }
    case ADCChannel::ADC2:
    {
        return CustomMath::doubleMap(((double)(this->adcValue2) / (8388607.0f)) * this->refValue,
                                     this->adcRange2.minValue,
                                     this->adcRange2.maxValue,
                                     this->adcRange2.minReal,
                                     this->adcRange2.maxReal);

        // return ((double)(this->adcValue2) / (8388607.0f)) * this->refValue;
        break;
    }
    case ADCChannel::Temperature:
    {
        return (float)temperatureValue * (273.15f + baseTemperature) / baseTemperatureADCValue - 273.15f;
        break;
    }
    case ADCChannel::ShortCircuit:
    {
        return shortCircuitValue;
        break;
    }

    default:
        break;
    }
}

void ZL_CS1238::setSleep(bool value)
{
    pinMode(pinSck, OUTPUT);
    digitalWrite(pinSck, value);
    sleepNow = value;
    vTaskDelay(300);
}

void ZL_CS1238::setADCRange(ADCChannel channel, double minValue, double maxValue, double minReal, double maxReal)
{
    switch (this->adcChannel)
    {
    case ADCChannel::ADC1:
    {
        this->adcRange1.minValue = minValue;
        this->adcRange1.maxValue = maxValue;
        this->adcRange1.minReal = minReal;
        this->adcRange1.maxReal = maxReal;
        break;
    }
    case ADCChannel::ADC2:
    {
        this->adcRange2.minValue = minValue;
        this->adcRange2.maxValue = maxValue;
        this->adcRange2.minReal = minReal;
        this->adcRange2.maxReal = maxReal;
        break;
    }
    case ADCChannel::Temperature:
    {
        break;
    }
    case ADCChannel::ShortCircuit:
    {
        break;
    }

    default:
        break;
    }
}
