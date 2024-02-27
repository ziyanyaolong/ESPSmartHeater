#ifndef SHADER_VALUES_H
#define SHADER_VALUES_H

#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <map>

#include "IPC/ReadWriteLock/ReadWriteLock.h"

/*
下个版本将加入指针直接操作功能，允许外部类保存void类型地址指针，在访问时传入void类型地址指针即可自动转换操作，牺牲极微量的内存，使得在有超多内存块时省下查找的CPU消耗，提高性能
*/
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
