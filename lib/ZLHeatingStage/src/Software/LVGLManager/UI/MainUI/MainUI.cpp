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
    FLLabel *label = new FLLabel(page);
    label->setPoint(0, 0);
    label->setSize(50, 100);
    label->setText("233");

    FLLabel *label2 = new FLLabel(page);
    label2->setPoint(100, 100);
    label2->setSize(50, 100);
    label2->setText("888");
}

void MainUI::loop()
{
}
