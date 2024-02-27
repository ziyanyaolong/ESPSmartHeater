#include "FLLabel.h"

FLLabel::FLLabel(WidgetAPI *parent)
    : WidgetAPI(parent)
{
    this->setLVGLObj(lv_label_create(this->parent()));

    this->initStyle();
}

FLLabel::FLLabel(lv_obj_t *parent)
    : WidgetAPI(parent)
{
    this->setLVGLObj(lv_label_create(this->parent()));

    this->initStyle();
}

FLLabel::~FLLabel()
{
}

void FLLabel::setText(const std::string& text)
{
    lv_label_set_text(this->getLVGLObj(), text.c_str());
}

std::string FLLabel::getText()
{
    return std::string(lv_label_get_text(this->getLVGLObj()));
}

void FLLabel::initStyle()
{
    style = new lv_style_t;
    lv_style_init(style); // 初始化style

    lv_style_set_radius(style, 5);                                             // 设置样式的圆角
    // lv_style_set_opa(style, LV_OPA_50);                                        // 设置样式背景透明度
    // lv_style_set_bg_color(style, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 1)); // 设置样式背景颜色
    lv_style_set_text_color(style, lv_palette_main(LV_PALETTE_LIGHT_BLUE));

    lv_style_set_border_color(style, lv_palette_main(LV_PALETTE_BLUE)); // 设置样式边框颜色
    lv_style_set_border_width(style, 3);                                // 设置样式边框宽度
    lv_style_set_border_opa(style, LV_OPA_30);                          // 设置样式边框透明度
    lv_style_set_border_side(style, LV_BORDER_SIDE_FULL);               // 设置样式边框显示范围

    lv_obj_add_style(this->getLVGLObj(), style, 0); // 将样式添加到对象中
}
