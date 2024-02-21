#ifndef ZLHS_READ_WRITE_LOCK_VALUE_H
#define ZLHS_READ_WRITE_LOCK_VALUE_H

#include "ReadWriteLock.h"

template <typename T>
class ReadWriteLockValue : private ReadWriteLock
{
private:
    T* typeValue = new T();
public:
    ReadWriteLockValue(ReadWriteLock::PriorityMode value = ReadWriteLock::PriorityMode::ReadPriority) : ReadWriteLock(value) {}
    virtual ~ReadWriteLockValue() {  }

    T read() { return *typeValue; }
    void write(const T* value) { memcpy(typeValue, value, sizeof(T)); }
};

#endif