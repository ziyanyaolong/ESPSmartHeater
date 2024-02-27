#ifndef ZL_LVGL_WIDGET_FL_LAYOUT_H
#define ZL_LVGL_WIDGET_FL_LAYOUT_H

#include <stdint.h>
#include <lvgl.h>

#include "Software/LVGLManager/API/WidgetAPI/WidgetAPI.h"

class FLLayout
{
private:
	WidgetAPI* widget = nullptr;

public:
	FLLayout();

	virtual ~FLLayout();

public:
	void setWidget(WidgetAPI* widget);

	WidgetAPI* getWidget();
};

#endif
