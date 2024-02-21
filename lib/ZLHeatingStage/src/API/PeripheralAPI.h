#ifndef ZLHS_PERIPHERALS_API_H
#define ZLHS_PERIPHERALS_API_H

#include "BaseSystemAPI.h"

class PeripheralAPI : public BaseSystemAPI
{
public:
    enum class OperateStage
    {
        None,
        Load,
        Unload
    };

private:
    friend class ZLHSSystem;

    OperateStage oStage = OperateStage::None;

protected:
    PeripheralAPI(String name) : BaseSystemAPI(name, BaseSystemAPI::APIType::Peripheral) {}
    virtual ~PeripheralAPI() {}

    virtual int load() = 0;
    virtual int unload() = 0;

    DEFINE_ZLHS_DATA_NO_DE(oStage, OperateStage, OperateStage, protected);
};

#endif