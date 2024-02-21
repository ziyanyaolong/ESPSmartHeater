#include "MutexLock.h"

MutexLock::MutexLock()
{
    this->mutex = xSemaphoreCreateMutex();
}

MutexLock::~MutexLock()
{
}

bool MutexLock::lock(TickType_t time)
{
    return xSemaphoreTake(mutex, time);
}

bool MutexLock::unlock()
{
    return xSemaphoreGive(mutex);
}

bool MutexLock::lockFromISR(BaseType_t * const pxHigherPriorityTaskWoken)
{
    return xSemaphoreTakeFromISR(mutex, pxHigherPriorityTaskWoken);
}

bool MutexLock::unlockFromISR(BaseType_t * const pxHigherPriorityTaskWoken)
{
    return xSemaphoreGiveFromISR(mutex, pxHigherPriorityTaskWoken);
}
