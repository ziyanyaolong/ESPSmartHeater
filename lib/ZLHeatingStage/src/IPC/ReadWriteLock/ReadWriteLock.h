#ifndef ZLHS_READ_WRITE_LOCK_H
#define ZLHS_READ_WRITE_LOCK_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>

class ReadWriteLock
{
    public:
    enum class PriorityMode
    {
        ReadPriority,
        WritePriority
    };
private:
    SemaphoreHandle_t rMutex;
    SemaphoreHandle_t wMutex;

    PriorityMode mode;

    uint32_t readNum = 0;

public:
    ReadWriteLock(PriorityMode value = PriorityMode::ReadPriority);
    virtual ~ReadWriteLock();

    void lockForRead();
    void lockForWrite();

    void unlockForRead();
    void unlockForWrite();
};

#endif