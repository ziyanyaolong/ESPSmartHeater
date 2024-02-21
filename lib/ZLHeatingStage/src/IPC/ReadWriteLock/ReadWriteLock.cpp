#include "ReadWriteLock.h"

ReadWriteLock::ReadWriteLock(PriorityMode value)
{
    rMutex = xSemaphoreCreateMutex();
    wMutex = xSemaphoreCreateMutex();

    this->mode = value;
}

ReadWriteLock::~ReadWriteLock()
{
    if (rMutex)
    {
        vSemaphoreDelete(rMutex);
        rMutex = nullptr;
    }
    if (wMutex)
    {
        vSemaphoreDelete(wMutex);
        wMutex = nullptr;
    }
}

void ReadWriteLock::lockForRead()
{
    xSemaphoreTake(rMutex, portMAX_DELAY);
    if (++readNum == 1)
    {
        xSemaphoreTake(wMutex, portMAX_DELAY);
    }
    xSemaphoreGive(rMutex);
}

void ReadWriteLock::lockForWrite()
{
    xSemaphoreTake(wMutex, portMAX_DELAY);
}

void ReadWriteLock::unlockForRead()
{
    xSemaphoreTake(rMutex, portMAX_DELAY);
    if (--readNum == 0)
    {
        xSemaphoreGive(wMutex);
    }
    xSemaphoreGive(rMutex);
}

void ReadWriteLock::unlockForWrite()
{
    xSemaphoreGive(wMutex);
}
