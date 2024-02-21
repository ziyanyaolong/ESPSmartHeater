#include "TemperatureControl.h"

TemperatureControl::TemperatureControl()
    : SoftwareAPI(ZLHS_EX_TO_STRING(TemperatureControl))
{
    this->setTCStackDepth(8192);
    this->setTCRunTimeDelay(1000);
}

TemperatureControl::~TemperatureControl()
{
}

void TemperatureControl::dynamicRangeConstraint(double inValue, DynamicValue &outValue)
{
    if (inValue < outValue.minValue)
    {
        inValue = outValue.minValue;
    }
    else if (inValue > outValue.maxValue)
    {
        inValue = outValue.maxValue;
    }

    outValue.realValue = inValue;
}

void TemperatureControl::dynamicMap(DynamicValue &outValue)
{
    outValue.realValue = CustomMath::doubleMap(this->temperatureDifference.realValue,
                                               this->temperatureDifference.minValue,
                                               this->temperatureDifference.maxValue,
                                               outValue.minValue, outValue.maxValue);
}

int TemperatureControl::init()
{
    // 从系统中获取驱动控制指针
    this->heater = ZLHSSystem::getRegAPICast<Heater>(ZLHS_EX_TO_STRING(Heater), BaseSystemAPI::APIType::Peripheral, UINT32_MAX);
    this->zl_CS1238 = ZLHSSystem::getRegAPICast<ZL_CS1238>(ZLHS_EX_TO_STRING(ZL_CS1238), BaseSystemAPI::APIType::Peripheral, UINT32_MAX);
    this->zl_INA219 = ZLHSSystem::getRegAPICast<ZL_INA219>(ZLHS_EX_TO_STRING(ZL_INA219), BaseSystemAPI::APIType::Peripheral, UINT32_MAX);

    // 如果获取不到则初始化失败，向系统提交初始化错误
    if (this->zl_CS1238 == nullptr)
    {
        return -1;
    }

    if (this->heater == nullptr)
    {
        return -1;
    }

    if (this->zl_INA219 == nullptr)
    {
        return -1;
    }

    // 向内存共享管理器申请内存区域
    MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, sizeof(double));

    // 设置参考电压为5V
    this->zl_CS1238->setREFType(5.0f);

    // PID初始化
    pid = new PID(&inValue, &outValue, &targetValue, pValue.baseValue, iValue.baseValue, dValue.baseValue, DIRECT);
    pid->SetMode(AUTOMATIC);
    pid->SetOutputLimits(heater->getPercentageMin(), heater->getPercentageMax());

    // 请根据实际的测量数值进行填入
    // (double)0.0462f
    this->zl_CS1238->setADCRange(ZL_CS1238::ADCChannel::ADC1, (double)0.0f, (double)0.01796875f, (double)-30.0f, (double)230.0f);

    // 更新设置
    this->zl_CS1238->setREFOUT(true, false);
    this->zl_CS1238->setADCChannel(ZL_CS1238::ADCChannel::ADC1, false);
    this->zl_CS1238->setPGAValue(ZL_CS1238::PGAValue::Value_128, false);
    this->zl_CS1238->setSPEEDValue(ZL_CS1238::SPEEDValue::Value_10Hz, false);
    this->zl_CS1238->writeReg();

    return 0;
}

void TemperatureControl::loop()
{
    MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<void *>(&targetValue));
    heater->setPercentage(targetValue);

    // PID计算
    // pid->Compute();
    auto iiiv = this->zl_CS1238->readADC();

    // vTaskDelay(1);

    // ZLHS_DEBUG_PRINTF("HEX-RG-T:%x, HEX-RG-S:%x\n", this->zl_CS1238->readReg(true).allValue, this->zl_CS1238->readReg().allValue);
    ZLHS_DEBUG_PRINTF("T:%d, %.4f, HEX T:%x\n", iiiv, this->zl_CS1238->readADCRealValue(false), iiiv);

    this->zl_INA219->defaultDebugPrint();
    // heater->setPercentage(60.0f);
    // 映射温差和PID相关参数
    // heater->setPercentage(this->outValue);
    // this->dynamicRangeConstraint(this->targetValue - zl_CS1238->readADCRealValue(false), this->temperatureDifference);
    // this->dynamicMap(pValue);
    // this->dynamicMap(iValue);
    // this->dynamicMap(dValue);

    // 从内存共享中同步数据
    // MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<void*>(&targetValue));

    // pid->SetTunings(pValue.baseValue, iValue.baseValue, dValue.baseValue);
}
