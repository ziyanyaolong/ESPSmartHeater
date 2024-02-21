#include "MemoryShader.h"

MemoryShader *MemoryShader::memoryShader = nullptr;

MemoryShader::MemoryShader()
{
}

MemoryShader::~MemoryShader()
{
}

bool MemoryShader::addNewMemorySpace(const std::string &name, size_t size)
{
    return this->addNewMemorySpace((const char *)name.c_str(), size);
}

bool MemoryShader::addNewMemorySpace(const char *name, size_t size)
{
    if (name == nullptr || name == "")
    {
        return false;
    }

    MemorySpace *ms = new MemorySpace();
    ms->data = (void *)malloc(size);
    ms->size = size;

    this->lock.lockForRead();

    auto iter = this->msList.find(name);

    if (iter != msList.end())
    {
        this->lock.unlockForRead();
        return true;
    }

    this->lock.unlockForRead();

    this->lock.lockForWrite();

    this->msList.insert(std::pair<std::string, MemorySpace *>(std::string(name), ms));

    this->lock.unlockForWrite();

    return true;
}

size_t MemoryShader::readData(const std::string &name, void *space)
{
    return this->readData((const char *)name.c_str(), space);
}

size_t MemoryShader::readData(const char *name, void *space)
{
    if (name == nullptr || name == "" || space == nullptr)
    {
        return 0;
    }

    this->lock.lockForRead();

    auto iter = this->msList.find(name);

    if (iter == msList.end())
    {
        this->lock.unlockForRead();
        return 0;
    }

    MemorySpace *ms = iter->second;

    if (ms == nullptr)
    {
        return 0;
    }

    this->lock.unlockForRead();

    ms->lock.lockForRead();
    memcpy(space, ms->data, ms->size);
    size_t realSize = ms->size;
    ms->lock.unlockForRead();

    return realSize;
}

void MemoryShader::writeData(const std::string &name, const void *value)
{
    this->writeData((const char *)name.c_str(), value);
}

void MemoryShader::writeData(const char *name, const void *value)
{
    if (name == nullptr || name == "" || value == nullptr)
    {
        return;
    }

    this->lock.lockForRead();

    auto iter = this->msList.find(name);

    if (iter == msList.end())
    {
        this->lock.unlockForRead();
        return;
    }

    MemorySpace *ms = iter->second;

    if (ms == nullptr)
    {
        return;
    }

    this->lock.unlockForRead();

    ms->lock.lockForWrite();
    memcpy(ms->data, value, ms->size);
    ms->lock.unlockForWrite();
}

MemoryShader *MemoryShader::instance()
{
    if (MemoryShader::memoryShader == nullptr)
    {
        MemoryShader::memoryShader = new MemoryShader();
    }

    return MemoryShader::memoryShader;
}
