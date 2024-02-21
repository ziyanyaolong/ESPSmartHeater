#ifndef ZL_CUSTOM_MATH_PERCENTAGE_MAP_H
#define ZL_CUSTOM_MATH_PERCENTAGE_MAP_H

#include "CustomMath/CustomMath.h"

template <typename T, typename PerT = float>
class PercentageMap
{
private:
    PerT percentageMin = 0.0f;
    PerT percentageMax = 100.0f;
    PerT percentageNow = 0.0f;

    T valueMin = 0;
    T valueMax = 0;
    T valueNow = 0;

public:
    PercentageMap() {}
    virtual ~PercentageMap() {}

    void setMapData(PerT perMin, PerT perMax, T vMin, T vMax)
    {
        this->percentageMin = perMin;
        this->percentageMax = perMax;
        this->percentageNow = perMin;

        this->valueMin = vMin;
        this->valueMax = vMax;
        this->valueNow = vMin;
    }

    T setMapValue(PerT pValue)
    {
        this->percentageNow = pValue;
        this->valueNow = CustomMath::tMap<PerT, PerT, T>(this->percentageNow, this->percentageMin, this->percentageMax, this->valueMin, this->valueMax);
        return this->valueNow;
    }

    T getMapValue()
    {
        return this->valueNow;
    }
};

#endif