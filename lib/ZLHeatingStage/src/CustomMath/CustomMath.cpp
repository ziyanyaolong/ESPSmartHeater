#include "CustomMath.h"

CustomMath::CustomMath()
{
}

CustomMath::~CustomMath()
{
}

float CustomMath::floatMap(float x, float in_min, float in_max, float out_min, float out_max)
{
    return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

double CustomMath::doubleMap(double x, double in_min, double in_max, double out_min, double out_max)
{
    return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}