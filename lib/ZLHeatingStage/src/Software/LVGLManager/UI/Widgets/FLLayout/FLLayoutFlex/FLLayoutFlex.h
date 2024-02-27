#ifndef ZL_LVGL_WIDGET_FL_LAYOUT_FLEX_H
#define ZL_LVGL_WIDGET_FL_LAYOUT_FLEX_H

#include <stdint.h>
#include <lvgl.h>

#include "Software/LVGLManager/UI/Widgets/FLLayout/FLLayout/FLLayout.h"

class FLLayoutFlex : public FLLayout
{
private:

public:
	FLLayoutFlex();

	virtual ~FLLayoutFlex();

public:

	void setFlexFlow(lv_flex_flow_t flexFlow);
	void setFlexGrowAll();
};

#endif
