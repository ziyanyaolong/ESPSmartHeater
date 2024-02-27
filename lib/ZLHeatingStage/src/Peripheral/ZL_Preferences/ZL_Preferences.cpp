#include "ZL_Preferences.h"

ZL_Preferences::ZL_Preferences()
    : PeripheralAPI(ZLHS_EX_TO_STRING(ZL_Preferences))
{
}

ZL_Preferences::~ZL_Preferences()
{
}

int ZL_Preferences::load()
{
    return 0;
}

int ZL_Preferences::unload()
{
    return 0;
}
