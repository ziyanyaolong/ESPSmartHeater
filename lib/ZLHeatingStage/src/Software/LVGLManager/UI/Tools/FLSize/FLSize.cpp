#include "FLSize.h"

FLSize::FLSize()
{
}

FLSize::FLSize(const FLSize *size)
{
    this->_width = size->_width;
    this->_height = size->_height;
}

FLSize::FLSize(lv_coord_t w, lv_coord_t h)
{
    this->_width = w;
    this->_height = h;
}

FLSize::~FLSize()
{
}

lv_coord_t FLSize::width() const
{
    return this->_width;
}

lv_coord_t FLSize::height() const
{
    return this->_height;
}