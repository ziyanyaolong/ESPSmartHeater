#ifndef ZLHS_MUTEX_LOCK_H
#define ZLHS_MUTEX_LOCK_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>

class MutexLock
{
public:
    MutexLock();
    virtual ~MutexLock();

private:
    SemaphoreHandle_t mutex;

public:
    bool lock(TickType_t time = portMAX_DELAY);
    bool unlock();
    bool lockFromISR(BaseType_t * const pxHigherPriorityTaskWoken = nullptr);
    bool unlockFromISR(BaseType_t * const pxHigherPriorityTaskWoken = nullptr);
};

#endif