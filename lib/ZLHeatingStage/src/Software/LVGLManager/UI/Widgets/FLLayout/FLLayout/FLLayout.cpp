#include "FLLayout.h"

FLLayout::FLLayout()
{
}
FLLayout::~FLLayout()
{
}

void FLLayout::setWidget(WidgetAPI* widget)
{
	this->widget = widget;
}

WidgetAPI* FLLayout::getWidget()
{
	return this->widget;
}
