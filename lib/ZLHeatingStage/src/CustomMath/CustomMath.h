#ifndef ZL_CUSTOM_MATH_H
#define ZL_CUSTOM_MATH_H

class CustomMath
{
private:
    CustomMath();
    virtual ~CustomMath();

public:
    static float floatMap(float x, float in_min, float in_max, float out_min, float out_max);
    static double doubleMap(double x, double in_min, double in_max, double out_min, double out_max);

    template <typename T1, typename T2 = T1, typename T3 = T2>
    static T3 tMap(T1 x, T2 in_min, T2 in_max, T3 out_min, T3 out_max)
    {
        return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
    }
};

#endif