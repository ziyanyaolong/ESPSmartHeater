#ifndef ZLHS_BLINKER_MANAGER_H
#define ZLHS_BLINKER_MANAGER_H

#define BLINKER_WIFI
#define BLINKER_PRINT Serial
#define BLINKER_ESP_SMARTCONFIG
#define BLINKER_WIFI
#define BLINKER_OTA_VERSION_CODE "0.0.1"

#include <Blinker.h>
#include <String>

#include <ShaderValues.h>

#include "API/SoftwareAPI.h"
#include "IPC/MemoryShader/MemoryShader.h"
// Blinker处理类

#define ZLHS_BLINKER_TOGGER

class BlinkerManager : public SoftwareAPI
{
private:
    String auth = "c2fd06431be1";
    double current = 0.0f;
    double voltage = 0.0f;
    double ttValue = 0.0f;
    double rtValue = 0.0f;

    BlinkerNumber *ttNumber;
    BlinkerNumber *realNumber;
    BlinkerNumber *aNumber;
    BlinkerNumber *vNumber;
    BlinkerSlider *slider;

private:
    static void buttonTTCallback(int32_t value)
    {
        double ttValue = (double)value;
        // 向内存共享中同步数据
        MemoryShader::instance()->writeData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<const void *>(&(ttValue)));
    }

    static void otaStatus(uint32_t load_size, uint32_t total_size)
    {
    }

    void updateToBlinker()
    {
        // 从内存共享中同步数据
        MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<void *>(&ttValue));
        MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_REAL_TEMPERATURE, static_cast<void *>(&rtValue));
        MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_VOLTAGE, static_cast<void *>(&voltage));
        MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_CURRENT, static_cast<void *>(&current));

        this->ttNumber->print(this->ttValue);
        this->realNumber->print(this->rtValue);

        this->vNumber->print(this->voltage);
        this->aNumber->print(this->current);
    }

public:
    explicit BlinkerManager()
        : SoftwareAPI(ZLHS_EX_TO_STRING(BlinkerManager))
    {
        this->setTCStackDepth(16384);
    }
    virtual ~BlinkerManager()
    {
    }

    virtual int init() override
    {
        MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, sizeof(double));
        MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_REAL_TEMPERATURE, sizeof(double));
        MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_VOLTAGE, sizeof(double));
        MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_CURRENT, sizeof(double));

        BLINKER_DEBUG.stream(Serial);
        // BLINKER_DEBUG.debugAll();
        Blinker.begin(auth.c_str());
        BlinkerUpdater.onProgress(&BlinkerManager::otaStatus);

        ttNumber = new BlinkerNumber("num-tempset");
        realNumber = new BlinkerNumber("num-tempreal");
        aNumber = new BlinkerNumber("num-a");
        vNumber = new BlinkerNumber("num-v");
        slider = new BlinkerSlider("ran-tt");

        slider->attach(&BlinkerManager::buttonTTCallback);

        return 0;
    }

    virtual void loop() override
    {
        Blinker.run();

        this->updateToBlinker();
    }
};

#endif