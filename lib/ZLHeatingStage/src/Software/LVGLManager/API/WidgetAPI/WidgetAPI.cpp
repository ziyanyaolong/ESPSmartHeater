#include "WidgetAPI.h"

WidgetAPI::WidgetAPI(WidgetAPI *parent)
{
    this->_parent = (parent == nullptr) ? lv_scr_act() : parent->obj;
}

WidgetAPI::WidgetAPI(lv_obj_t *parent)
{
    this->_parent = (parent == nullptr) ? lv_scr_act() : parent;
}

WidgetAPI::~WidgetAPI()
{
    this->_parent = nullptr;
}

void WidgetAPI::setLVGLObj(lv_obj_t *obj)
{
    this->obj = obj;
}

lv_obj_t *WidgetAPI::getLVGLObj()
{
    return this->obj;
}

void WidgetAPI::setSize(const FLSize &size)
{
    lv_obj_set_size(this->obj, size.width(), size.height());
}

void WidgetAPI::setSize(lv_coord_t w, lv_coord_t h)
{
    lv_obj_set_size(this->obj, w, h);
}

void WidgetAPI::setPoint(const FLPoint &point)
{
    lv_obj_set_pos(this->obj, point.x(), point.y());
}

void WidgetAPI::setPoint(lv_coord_t x, lv_coord_t y)
{
    lv_obj_set_pos(this->obj, x, y);
}

lv_obj_t *WidgetAPI::parent()
{
    return this->_parent;
}

FLSize WidgetAPI::size()
{
    return FLSize(this->obj->coords.x2, this->obj->coords.y2);
}

FLPoint WidgetAPI::point()
{
    return FLPoint(this->obj->coords.x1, this->obj->coords.y1);
}
