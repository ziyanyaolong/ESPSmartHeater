#include "ZLHSSystem.h"

ZLHSSystem *ZLHSSystem::zlhsSystem;

ZLHSSystem::ZLHSSystem()
{
}

ZLHSSystem::~ZLHSSystem()
{
}

void ZLHSSystem::taskFuncPeri(void *userdata)
{
    if (userdata == nullptr)
    {
        vTaskDelete(nullptr);
    }

    ZLHSSystem *system = static_cast<ZLHSSystem *>(userdata);

    if (system == nullptr)
    {
        vTaskDelete(nullptr);
    }

    while (true)
    {
        vTaskDelay(1000);

        system->lockPRL.lockForRead();

        if (system->periReadyList.empty())
        {
            system->lockPRL.unlockForRead();
            continue;
        }

        system->lockPRL.unlockForRead();

        int rValue = 0;
        PeripheralAPI *api = nullptr;

        system->lockPRL.lockForWrite();

        for (auto iter = system->periReadyList.begin(); iter != system->periReadyList.end(); iter++)
        {
            api = static_cast<PeripheralAPI *>(*iter);
            switch (api->getOperateStage())
            {
            case PeripheralAPI::OperateStage::Load:
            {
                if (!(api->getInitOk()))
                {
                    rValue = api->load();
                    if (rValue == 0)
                    {
                        api->setInitOk(true);
                        ZLHS_DEBUG_PRINTF("%s Peri Init OK\r\n", api->getTCName());
                    }
                    else
                    {
                        ZLHS_DEBUG_PRINTF("Error:%s Peri Init Error %d\r\n", api->getTCName(), rValue);
                    }
                }
                break;
            }

            default:
                break;
            }
            api->setOperateStage(PeripheralAPI::OperateStage::None);
        }

        system->periReadyList.clear();

        system->lockPRL.unlockForWrite();
    }

    vTaskDelete(nullptr);
}

void ZLHSSystem::taskFuncSoft(void *userdata)
{
    if (userdata == nullptr)
    {
        ZLHS_DEBUG_PRINTF("Error:Empty Soft Pointer\r\n");

        vTaskDelete(nullptr);
    }

    SoftwareAPI *task = static_cast<SoftwareAPI *>(userdata);

    if (task == nullptr)
    {
        ZLHS_DEBUG_PRINTF("Error:Soft Pointer Cast Error\r\n");

        vTaskDelete(nullptr);
    }

    auto rValue = task->init();

    if (rValue != 0)
    {
        ZLHS_DEBUG_PRINTF("Error:%s Soft Init Error %d\r\n", task->getTCName(), rValue);

        vTaskDelete(nullptr);
    }

    task->setInitOk(true);

    ZLHS_DEBUG_PRINTF("%s Soft Init OK\r\n", task->getTCName());

    while (true)
    {
        task->loop();
        vTaskDelay(task->getTCRunTimeDelay());
    }
}

bool ZLHSSystem::regNewProcess(BaseSystemAPI *api)
{
    if (api == nullptr)
    {
        return false;
    }

    switch (api->getAPIType())
    {
    case BaseSystemAPI::APIType::Peripheral:
    {
        ZLHS_DEBUG_PRINTF("PERI:%s\n", api->getTCName().c_str());
        lockPL.lockForRead();
        auto iter = this->periList.find(std::string(api->getTCName().c_str()));
        lockPL.unlockForRead();

        if (iter != this->periList.end())
        {
            return false;
        }

        ((PeripheralAPI *)api)->setOperateStage(PeripheralAPI::OperateStage::Load);

        lockPL.lockForWrite();
        this->periList.insert(std::pair<std::string, PeripheralAPI *>(std::string(api->getTCName().c_str()), (PeripheralAPI *)api));
        lockPL.unlockForWrite();

        lockPRL.lockForWrite();
        this->periReadyList.push_back((PeripheralAPI *)api);
        lockPRL.unlockForWrite();

        break;
    }
    case BaseSystemAPI::APIType::Software:
    {
        ZLHS_DEBUG_PRINTF("SOFT:%s\n", api->getTCName().c_str());

        lockSL.lockForWrite();
        this->softList.push_back((SoftwareAPI *)api);
        lockSL.unlockForWrite();

        api->setTCReturned(xTaskCreatePinnedToCore(&ZLHSSystem::taskFuncSoft,
                                                   api->getTCName().c_str(),
                                                   api->getTCStackDepth(),
                                                   static_cast<void *const>(api),
                                                   api->getTCPriority(),
                                                   api->getTCTaskHandle(),
                                                   ZLHS_SOFTWARE_CORE));
        break;
    }
    default:
        return false;
        break;
    }

    return true;
}

BaseSystemAPI *ZLHSSystem::getRegAPI(String name, BaseSystemAPI::APIType type, uint32_t waiteTime)
{
    uint32_t oldTime = millis();
    uint32_t newTime = oldTime;

    BaseSystemAPI *tAPI = nullptr;

    while ((waiteTime == UINT32_MAX) || ((newTime - oldTime) <= waiteTime))
    {
        if (tAPI == nullptr)
        {
            switch (type)
            {
            case BaseSystemAPI::APIType::Peripheral:
            {
                ZLHSSystem::zlhsSystem->lockPL.lockForRead();
                auto iter = ZLHSSystem::zlhsSystem->periList.find(std::string(name.c_str()));

                if (iter != ZLHSSystem::zlhsSystem->periList.end())
                {
                    tAPI = iter->second;
                }
                else
                {
                    ZLHSSystem::zlhsSystem->lockPL.unlockForRead();
                    newTime = millis();
                    vTaskDelay(500);
                    continue;
                }
                ZLHSSystem::zlhsSystem->lockPL.unlockForRead();

                break;
            }
            case BaseSystemAPI::APIType::Software:
            {
                // 不允许直接访问软件应用，请使用IPC的通讯方式
                return nullptr;
                break;
            }
            default:
                break;
            }
        }

        if (tAPI->getInitOk())
        {
            break;
        }

        vTaskDelay(500);
    }

    return tAPI;
}

void ZLHSSystem::start()
{
    if (zlhsSystem == nullptr)
    {
        ZLHS_DEBUG_PRINTLN("Create New ZLHSSystem!");
        zlhsSystem = new ZLHSSystem();

        xTaskCreatePinnedToCore(&ZLHSSystem::taskFuncPeri,
                                "ZLHSSystemPeri",
                                16384,
                                static_cast<void *const>(zlhsSystem),
                                1,
                                nullptr,
                                ZLHS_PERIPHERAL_CORE);
    }
}
