#ifndef ZLHS_CODEGENERATE_H
#define ZLHS_CODEGENERATE_H

#include "ZLHSConfig.h"
#include "AllPin.h"

#include <stdint.h>

// 以下宏定义为Getter和Setter两种生成器，可以对相应的数据进行自动配置，类似于Java的@Gatter、@Setter和@Data功能
#define DEFINE_ZLHS_MU_DATA(name, funName, type)                  \
private:                                                          \
    SemaphoreHandle_t name##Mutex;                                \
    type name;                                                    \
                                                                  \
public:                                                           \
    type get##funName()                                           \
    {                                                             \
        xSemaphoreTake(name##Mutex, portMAX_DELAY);               \
        type tValue = name;                                       \
        xSemaphoreGive(name##Mutex);                              \
        return tValue;                                            \
    }                                                             \
    void set##funName(type value)                                 \
    {                                                             \
        if (xSemaphoreTake(name##Mutex, portMAX_DELAY) == pdTRUE) \
        {                                                         \
            name = value;                                         \
            xSemaphoreGive(name##Mutex);                          \
        }                                                         \
    }

#define DEFINE_ZLHS_INIT_DATA(name) \
    name##Mutex = xSemaphoreCreateMutex();

#define DEFINE_ZLHS_DATA(name, funName, type) \
private:                                      \
    type name;                                \
                                              \
public:                                       \
    inline type get##funName()                \
    {                                         \
        return name;                          \
    }                                         \
    inline void set##funName(type value)      \
    {                                         \
        name = value;                         \
    }

#define DEFINE_ZLHS_DATA_NO_DE(name, funName, type, visitType) \
    visitType:                                                 \
    inline type get##funName()                                 \
    {                                                          \
        return name;                                           \
    }                                                          \
    inline void set##funName(type value)                       \
    {                                                          \
        name = value;                                          \
    }

#define DEFINE_ZLHS_DATA_NO_DE_QUOTE(name, funName, type, visitType) \
    visitType:                                                       \
    inline const type &get##funName()                                \
    {                                                                \
        return name;                                                 \
    }                                                                \
    inline void set##funName(const type &value)                      \
    {                                                                \
        name = value;                                                \
    }

#define ZLHS_EX_TO_STRING(type) (#type)

#define csuper(classType) ((classType *)(this))

#ifdef ZLHS_DEBUG_MODE_ON
#define ZLHS_DEBUG_CLASS Serial
#define ZLHS_DEBUG_PRINT(...) ZLHS_DEBUG_CLASS.print(__VA_ARGS__);
#define ZLHS_DEBUG_PRINTLN(...) ZLHS_DEBUG_CLASS.println(__VA_ARGS__);
#define ZLHS_DEBUG_PRINTF(...) ZLHS_DEBUG_CLASS.printf(__VA_ARGS__);
#else
#define ZLHS_DEBUG_CLASS
#define ZLHS_DEBUG_PRINT(...)
#define ZLHS_DEBUG_PRINTLN(...)
#define ZLHS_DEBUG_PRINTF(...)
#endif

#define ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE ("TargetTemperature")
#define ZLHS_MEMORY_SPACE_NAME_REAL_TEMPERATURE ("RealTemperature")
#define ZLHS_MEMORY_SPACE_NAME_CURRENT ("current")
#define ZLHS_MEMORY_SPACE_NAME_VOLTAGE ("voltage")

namespace ZLTypes
{
    typedef int8_t PINType;
    typedef uint32_t PMType;
    typedef uint32_t SPIDType;
};

#endif