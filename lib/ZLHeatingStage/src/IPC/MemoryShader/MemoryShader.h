#ifndef SHADER_VALUES_H
#define SHADER_VALUES_H

#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <map>

#include "IPC/ReadWriteLock/ReadWriteLock.h"

class MemoryShader
{
private:
    typedef struct MemorySpace
    {
        size_t size = 0;
        ReadWriteLock lock;
        void *data = nullptr;
    } MemorySpace;

    ReadWriteLock lock;
    static MemoryShader *memoryShader;
    std::map<std::string, MemorySpace*> msList;

public:
    MemoryShader();
    virtual ~MemoryShader();

    bool addNewMemorySpace(const std::string &name, size_t size);
    bool addNewMemorySpace(const char *name, size_t size);

    size_t readData(const std::string &name, void* space);
    size_t readData(const char *name, void* space);

    void writeData(const std::string &name, const void *value);
    void writeData(const char *name, const void *value);

    static MemoryShader *instance();
};

#endif
