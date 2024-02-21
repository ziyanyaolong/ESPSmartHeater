#ifndef ZLHS_SOFTWARE_API_H
#define ZLHS_SOFTWARE_API_H

#include "BaseSystemAPI.h"

class SoftwareAPI : public BaseSystemAPI
{
private:
    friend class ZLHSSystem;

protected:
    SoftwareAPI(String name) : BaseSystemAPI(name, BaseSystemAPI::APIType::Software) {}
    virtual ~SoftwareAPI() {}

    virtual int init() = 0;
    virtual void loop() = 0;
};

#endif