#include "MainUI.h"

MainUI::MainUI()
{
}

MainUI::~MainUI()
{
}

uint8_t MainUI::init()
{
    return 0;
}

void MainUI::setupUI(lv_obj_t *page)
{
    // 向内存共享管理器申请内存区域
    MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, sizeof(double));
    MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_CURRENT, sizeof(double));
    MemoryShader::instance()->addNewMemorySpace(ZLHS_MEMORY_SPACE_NAME_VOLTAGE, sizeof(double));

    fLayoutPage = new FLLayoutFlex();
    fLayoutVoltage = new FLLayoutFlex();
    fLayoutCurrent = new FLLayoutFlex();
    fLayoutPower = new FLLayoutFlex();

    widgetVoltage = new FLWidget(page);
    widgetCurrent = new FLWidget(page);
    widgetPower = new FLWidget(page);

    fLayoutVoltage->setWidget(widgetVoltage);
    fLayoutCurrent->setWidget(widgetCurrent);
    fLayoutPower->setWidget(widgetPower);

    fLayoutVoltage->setFlexFlow(LV_FLEX_FLOW_ROW_WRAP);
    fLayoutCurrent->setFlexFlow(LV_FLEX_FLOW_ROW_WRAP);
    fLayoutPower->setFlexFlow(LV_FLEX_FLOW_ROW_WRAP);

    fLayoutVoltage->setFlexGrowAll();
    fLayoutCurrent->setFlexGrowAll();
    fLayoutPower->setFlexGrowAll();

    // lv_style_t* style = new lv_style_t;

    // lv_style_init(style);
    // lv_style_set_bg_opa(style, 0);
    // lv_style_set_radius(style, 0);
    // lv_style_set_pad_all(style, 0);

    // lv_obj_set_flex_flow(page, LV_FLEX_FLOW_COLUMN_WRAP);

    // lv_obj_t *cont = lv_obj_create(page); // 基于屏幕创建一个容器对象

    // lv_obj_add_style(cont, style, 0);

    // lv_obj_set_flex_grow(cont, 1);
    // lv_obj_set_width(cont, lv_pct(100));

    // lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);

    // lv_obj_t* cont2 = lv_obj_create(page); // 基于屏幕创建一个容器对象

    // lv_obj_set_flex_grow(cont2, 2);
    // lv_obj_set_width(cont2, lv_pct(100));
    // lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);

    // lv_obj_set_size(cont, 300, 220);                          // 设置对象宽度和高度
    // lv_obj_set_align(cont, LV_ALIGN_TOP_LEFT);

    // lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    // lv_obj_set_size(cont, 300, 75);
    // lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 2);
    // lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    // lv_obj_set_flex_align(cont, LV_FLEX_FLOW_ROW_WRAP);
    //  lv_obj_center(cont);

    // lv_obj_add_style(cont, &style, 0);

    labelVoltage1 = new FLLabel(widgetVoltage);
    // labelVoltage1->setPoint(0, 0);
    labelVoltage1->setText("Voltage:");

    labelVoltage2 = new FLLabel(widgetVoltage);
    // labelVoltage2->setPoint(100, 0);
    labelVoltage2->setText("0V");

    labelCurrent1 = new FLLabel(widgetCurrent);
    // labelCurrent1->setPoint(0, 50);
    labelCurrent1->setText("Current:");

    labelCurrent2 = new FLLabel(widgetCurrent);
    // labelCurrent2->setPoint(100, 50);
    labelCurrent2->setText("0A");

    hPower1 = new FLLabel(widgetPower);
    // hPower1->setPoint(0, 100);
    hPower1->setText("HPower:");

    hPower2 = new FLLabel(widgetPower);
    // hPower2->setPoint(100, 100);
    hPower2->setText("0%");

    FLWidget *pageWidget = FLWidget::objCaseToWidget(page);

    fLayoutPage->setWidget(pageWidget);
    fLayoutPage->setFlexFlow(LV_FLEX_FLOW_COLUMN);
    fLayoutPage->setFlexGrowAll();

    // delete pageWidget;
}

void MainUI::loop()
{
    MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_VOLTAGE, static_cast<void *>(&eValue));
    labelVoltage2->setText(std::to_string(eValue) + std::string("V"));

    MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_CURRENT, static_cast<void *>(&eValue));
    labelCurrent2->setText(std::to_string(eValue) + std::string("A"));

    MemoryShader::instance()->readData(ZLHS_MEMORY_SPACE_NAME_TARGET_TEMPERATURE, static_cast<void *>(&eValue));
    hPower2->setText(std::to_string(eValue));
}
