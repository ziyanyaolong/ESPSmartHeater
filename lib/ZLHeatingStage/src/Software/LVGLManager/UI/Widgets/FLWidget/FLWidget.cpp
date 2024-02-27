#include "FLWidget.h"

FLWidget::FLWidget(lv_obj_t* parent, int flag)
	: WidgetAPI(parent)
{
}

FLWidget::FLWidget(WidgetAPI* parent)
	: WidgetAPI(parent)
{
	this->setLVGLObj(lv_obj_create(this->parent()));
}

FLWidget::FLWidget(lv_obj_t* parent)
	: WidgetAPI(parent)
{
	this->setLVGLObj(lv_obj_create(this->parent()));
}

FLWidget::~FLWidget()
{
}

FLWidget* FLWidget::objCaseToWidget(lv_obj_t* obj, lv_obj_t* parent)
{
	FLWidget* tempW = new FLWidget(parent, 0);

	tempW->setLVGLObj(obj);

	return tempW;
}