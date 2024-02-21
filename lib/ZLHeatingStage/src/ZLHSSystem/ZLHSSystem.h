#ifndef ZLHS_SYSTEM_H
#define ZLHS_SYSTEM_H

#include <list>
#include <map>

#include <API/PeripheralAPI.h>
#include <API/SoftwareAPI.h>

#include "IPC/ReadWriteLock/ReadWriteLock.h"

class ZLHSSystem
{
public:
    explicit ZLHSSystem();
    virtual ~ZLHSSystem();

private:
    static ZLHSSystem *zlhsSystem;
    std::list<PeripheralAPI *> periReadyList;
    std::map<std::string, PeripheralAPI *> periList;
    std::list<SoftwareAPI *> softList;

    ReadWriteLock lockPRL;
    ReadWriteLock lockPL;
    ReadWriteLock lockSL;

private:
    static void taskFuncPeri(void *userdata);
    static void taskFuncSoft(void *userdata);

    bool regNewProcess(BaseSystemAPI *api);

public:
    template <typename ClassType>
    inline static ClassType *regNew()
    {
        if (ZLHSSystem::zlhsSystem == nullptr)
        {
            return nullptr;
        }
        auto tClassObject = new ClassType();

        if (tClassObject == nullptr)
        {
            return nullptr;
        }

        if (!(ZLHSSystem::zlhsSystem->regNewProcess(static_cast<BaseSystemAPI *>(tClassObject))))
        {
            delete tClassObject;
            tClassObject = nullptr;
        }

        return tClassObject;
    }

    static BaseSystemAPI *getRegAPI(String name, BaseSystemAPI::APIType type, uint32_t waiteTime = UINT32_MAX);

    template <typename ClassType>
    static ClassType *getRegAPICast(String name, BaseSystemAPI::APIType type, uint32_t waiteTime = UINT32_MAX)
    {
        return static_cast<ClassType *>(ZLHSSystem::getRegAPI(name, type, waiteTime));
    }

    static void start();
};

typedef ZLHSSystem ZLHeatingStage;

#endif