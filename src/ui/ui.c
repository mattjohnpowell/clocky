// src/ui/ui.c
#include "ui.h"
#include "ui_helpers.h"

// SCREEN: ui_scrMain
void ui_scrMain_screen_init(void);
lv_obj_t * ui_scrMain;
lv_obj_t * ui_imgBackground;
lv_obj_t * ui_imgBedtimeNotification;
lv_obj_t * ui_imgSpiderman;
lv_obj_t * ui_imgSpidermanSleep;
lv_obj_t * ui_pnlMain;
lv_obj_t * ui_lblClock;
lv_obj_t * ui_lblDate;
lv_obj_t * ui_Rotate;
lv_obj_t * ui_Label1;
lv_obj_t * ui_sun;
lv_obj_t * ui_moon;
lv_obj_t * ui_lblWakeTime;
lv_obj_t * ui_lblSleepTime;
lv_obj_t * ui____initial_actions0;

void ui_event_Rotate(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        OnRotateClicked(e);
    }
}

void ui_init(void)
{
    lv_disp_t * dispp = lv_display_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, 
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_RED),
        false, 
        LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_scrMain_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_scrMain);
}