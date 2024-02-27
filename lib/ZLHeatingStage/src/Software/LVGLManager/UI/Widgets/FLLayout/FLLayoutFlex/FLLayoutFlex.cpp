#include "FLLayoutFlex.h"

FLLayoutFlex::FLLayoutFlex()
{
}


FLLayoutFlex::~FLLayoutFlex()
{
}

void FLLayoutFlex::setFlexFlow(lv_flex_flow_t flexFlow)
{
	lv_obj_set_flex_flow(this->getWidget()->getLVGLObj(), flexFlow);
	lv_obj_set_flex_align(this->getWidget()->getLVGLObj(), LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
}

void FLLayoutFlex::setFlexGrowAll()
{
	int32_t i = 0;
	lv_obj_t* mainObj = this->getWidget()->getLVGLObj();

	for (lv_obj_t* cObj = lv_obj_get_child(mainObj, i);
		cObj != nullptr;
		++i, cObj = lv_obj_get_child(mainObj, i))
	{
		lv_obj_set_flex_grow(cObj, 1);
		lv_obj_set_width(cObj, lv_pct(100));
		lv_obj_set_height(cObj, lv_pct(100));
	}
}
