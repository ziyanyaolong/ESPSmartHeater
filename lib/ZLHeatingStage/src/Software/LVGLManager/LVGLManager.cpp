#include "LVGLManager.h"

LVGLManager::LVGLManager()
    : SoftwareAPI(ZLHS_EX_TO_STRING(LVGLManager))
{
    this->setTCRunTimeDelay(5);
    this->setTCStackDepth(32768);
    this->setTCPriority(10);
}

LVGLManager::~LVGLManager()
{
}

lv_obj_t *LVGLManager::createNewPage(lv_obj_t *parent, lv_obj_t *uiPage, const std::string &name)
{
    lv_obj_t *obj = lv_menu_cont_create(parent);

    lv_obj_t *label = lv_label_create(obj);
    lv_label_set_text(label, name.c_str());
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    // lv_obj_set_flex_grow(label, 1);

    lv_obj_t *page = lv_menu_page_create(menu, nullptr);

	lv_style_t* style = new lv_style_t;
	lv_style_init(style); // 初始化style

	lv_style_set_radius(style, 5);                                             // 设置样式的圆角
	//lv_style_set_opa(style, LV_OPA_50);                                        // 设置样式背景透明度
	//lv_style_set_bg_color(style, lv_palette_lighten(LV_PALETTE_BLUE_GREY, 1)); // 设置样式背景颜色

	lv_style_set_border_color(style, lv_palette_main(LV_PALETTE_BLUE)); // 设置样式边框颜色
	lv_style_set_border_width(style, 3);                                // 设置样式边框宽度
	lv_style_set_border_opa(style, LV_OPA_30);                          // 设置样式边框透明度
	lv_style_set_border_side(style, LV_BORDER_SIDE_FULL);               // 设置样式边框显示范围

	lv_obj_add_style(page, style, 0); // 将样式添加到对象中

    //lv_obj_set_width(page, LV_SIZE_CONTENT);
    //lv_obj_set_height(page, LV_SIZE_CONTENT);

    // lv_obj_set_size(page, lv_disp_get_hor_res(nullptr), lv_disp_get_ver_res(nullptr));
    lv_obj_center(page);

    // lv_obj_set_style_pad_all(page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    // lv_menu_separator_create(page);

    lv_menu_set_load_page_event(menu, obj, page);

    return page;
}

int LVGLManager::init()
{
#ifdef ZLHS_SIMULATOR_DEBUG
	lv_init();

	if (!lv_win32_init(
		GetModuleHandleW(NULL),
		SW_SHOW,
		800,
		480,
		LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))
	{
	}
#else
    lvglDriver = ZLHSSystem::getRegAPICast<LVGLDriver>(ZLHS_EX_TO_STRING(LVGLDriver), BaseSystemAPI::APIType::Peripheral, UINT32_MAX);
#endif
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_STATE_DEFAULT);

    // 菜单创建
    menu = lv_menu_create(lv_scr_act());
    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if (lv_color_brightness(bg_color) > 127)
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
    }
    else
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }

    lv_obj_set_size(menu, lv_disp_get_hor_res(nullptr), lv_disp_get_ver_res(nullptr));
    lv_obj_center(menu);

    // 设置为菜单选项栏
    menuSub = lv_menu_page_create(menu, nullptr);
    lv_menu_set_sidebar_page(menu, menuSub);
    lv_obj_set_style_pad_hor(menuSub, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(menuSub);

    // 设置为主页面显示
    menuWidget = lv_menu_page_create(menu, nullptr);
    lv_menu_set_page(menu, menuWidget);
    lv_obj_set_style_pad_hor(menuWidget, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(menuWidget);

    section = lv_menu_section_create(menuSub);

    this->regNewUI<MainUI>();

    return 0;
}

void LVGLManager::loop()
{
    newTime = millis();

    for (auto iter = uiList.begin(); iter != uiList.end(); iter++)
    {
        switch ((*iter)->lState)
        {
        case LVGLUIAPI::APILoadStage::InitToDisplayUI:
        {
            ZLHS_DEBUG_PRINTLN("LVGL:UI InitToDisplayUI OK");
            (*iter)->init();
            (*iter)->page = lv_scr_act();
            (*iter)->setupUI((*iter)->page);
            (*iter)->lState = LVGLUIAPI::APILoadStage::Loop;
            break;
        }
        case LVGLUIAPI::APILoadStage::Init:
        {
            ZLHS_DEBUG_PRINTLN("LVGL:UI Init OK");
            (*iter)->init();
            (*iter)->page = createNewPage(this->section, this->menuSub, (*iter)->getName());
            (*iter)->setupUI((*iter)->page);
            (*iter)->lState = LVGLUIAPI::APILoadStage::Loop;
            break;
        }

        case LVGLUIAPI::APILoadStage::Loop:
        {
            if (((newTime) - ((*iter)->oldTime)) > ((*iter)->delayTimeMS))
            {
                (*iter)->oldTime = newTime;
                (*iter)->loop();
            }
            break;
        }

        default:
            break;
        }
    }

    lv_task_handler();
}

void LVGLManager::regNewUI(LVGLUIAPI *api)
{
    uiList.push_back(api);
}
