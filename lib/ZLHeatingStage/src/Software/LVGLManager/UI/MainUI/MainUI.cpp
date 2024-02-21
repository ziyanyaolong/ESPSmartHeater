#include "MainUI.h"

MainUI::MainUI()
{
}

MainUI::~MainUI()
{
}

uint8_t MainUI::init()
{
    this->zl_INA219 = ZLHSSystem::getRegAPICast<ZL_INA219>(ZLHS_EX_TO_STRING(ZL_INA219), BaseSystemAPI::APIType::Peripheral, UINT32_MAX);

    // 如果获取不到则初始化失败，向UI系统提交初始化错误
    if (this->zl_INA219 == nullptr)
    {
        return -1;
    }

    return 0;
}

void MainUI::setupUI(lv_obj_t *page)
{
    labelVoltage1 = new FLLabel(page);
    labelVoltage1->setPoint(0, 0);
    labelVoltage1->setSize(100, 50);
    labelVoltage1->setText("Voltage:");

    labelVoltage2 = new FLLabel(page);
    labelVoltage2->setPoint(100, 0);
    labelVoltage2->setSize(100, 50);
    labelVoltage2->setText("0V");

    labelCurrent1 = new FLLabel(page);
    labelCurrent1->setPoint(0, 50);
    labelCurrent1->setSize(100, 50);
    labelCurrent1->setText("Current:");

    labelCurrent2 = new FLLabel(page);
    labelCurrent2->setPoint(100, 50);
    labelCurrent2->setSize(100, 50);
    labelCurrent2->setText("0A");

    hPower1 = new FLLabel(page);
    hPower1->setPoint(0, 100);
    hPower1->setSize(100, 50);
    hPower1->setText("HPower:");

    hPower2 = new FLLabel(page);
    hPower2->setPoint(100, 100);
    hPower2->setSize(100, 50);
    hPower2->setText("0%");
}

void MainUI::loop()
{
    static double value = 0.0f;

    labelVoltage2->setText(std::to_string(zl_INA219->getVoltageV()) + std::string("V"));
    labelCurrent2->setText(std::to_string(zl_INA219->getCurrentA()) + std::string("A"));

    MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<void *>(&value));

    hPower2->setText(std::to_string(value));
}
