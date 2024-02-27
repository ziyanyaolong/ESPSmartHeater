#ifndef ZL_LVGL_WIDGET_FL_WIDGET_H
#define ZL_LVGL_WIDGET_FL_WIDGET_H

#include <stdint.h>
#include <lvgl.h>

#include "Software/LVGLManager/API/WidgetAPI/WidgetAPI.h"

class FLWidget : public WidgetAPI
{
private:
	FLWidget(lv_obj_t* parent, int flag);

public:
	FLWidget(WidgetAPI* parent = nullptr);
	FLWidget(lv_obj_t* parent);

	virtual ~FLWidget();

public:
	static FLWidget* objCaseToWidget(lv_obj_t* obj, lv_obj_t* parent = nullptr);
};

#endif
