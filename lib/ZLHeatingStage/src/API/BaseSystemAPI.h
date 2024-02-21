#ifndef ZLHS_BASE_SYSTEM_API_H
#define ZLHS_BASE_SYSTEM_API_H

#include <CodeGenerate/CodeGenerate.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

class BaseSystemAPI
{
public:
    typedef struct ThreadConfig
    {
        String name = "";
        uint32_t stackDepth = 4096;
        void *parameters = nullptr;
        UBaseType_t priority = 1;
        TaskHandle_t *taskHandle = nullptr;
        short coreID = -1;
        BaseType_t returned = 0;
        TickType_t runTimeDelay = 50;
    } ThreadConfig;

    enum class APIType
    {
        Peripheral,
        Software,
        Unknown
    };

private:
    friend class ZLHSSystem;
    unsigned int spid = 0;
    bool initOk = false;
    ThreadConfig threadConfig;
    APIType apiType = APIType::Unknown;

protected:
    explicit BaseSystemAPI(String taskName, APIType type)
    {
        threadConfig.name = taskName;
        apiType = type;
    }

    virtual ~BaseSystemAPI() {}

public:
    DEFINE_ZLHS_DATA_NO_DE(initOk, InitOk, bool, private);
    DEFINE_ZLHS_DATA_NO_DE(threadConfig.name, TCName, String, private);
    DEFINE_ZLHS_DATA_NO_DE(threadConfig.taskHandle, TCTaskHandle, TaskHandle_t *, private);
    DEFINE_ZLHS_DATA_NO_DE(threadConfig.coreID, TCCoreID, short, private);
    DEFINE_ZLHS_DATA_NO_DE(threadConfig.returned, TCReturned, BaseType_t, private);
    DEFINE_ZLHS_DATA_NO_DE(apiType, APIType, APIType, protected);
    DEFINE_ZLHS_DATA_NO_DE(threadConfig.stackDepth, TCStackDepth, uint32_t, protected);
    DEFINE_ZLHS_DATA_NO_DE(threadConfig.priority, TCPriority, UBaseType_t, protected);
    DEFINE_ZLHS_DATA_NO_DE(threadConfig.runTimeDelay, TCRunTimeDelay, TickType_t, protected);
};

#endif