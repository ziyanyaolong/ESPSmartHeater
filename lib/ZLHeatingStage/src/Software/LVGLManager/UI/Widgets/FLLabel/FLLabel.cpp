#include "FLLabel.h"

FLLabel::FLLabel(WidgetAPI *parent)
    : WidgetAPI(parent)
{
    label = lv_label_create(this->parent());
    this->setLVGLObj(label);
}

FLLabel::FLLabel(lv_obj_t *parent)
    : WidgetAPI(parent)
{
    label = lv_label_create(this->parent());
    this->setLVGLObj(label);
}

FLLabel::~FLLabel()
{
    if (label)
    {
        lv_obj_del(label);
        label = nullptr;
    }
}

void FLLabel::setText(std::string text)
{
    lv_label_set_text(label, text.c_str());
}

std::string FLLabel::getText()
{
    return std::string(lv_label_get_text(label));
}
