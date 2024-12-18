// src/ui/ui.h
#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_events.h"
#include "lv_font_montserrat_120.h"
#include "spiderman.h"
#include "spidermansleep.h"

    // SCREEN: ui_scrMain
    void ui_scrMain_screen_init(void);
    extern lv_obj_t *ui_scrMain;
    extern lv_obj_t *ui_imgBackground;
    extern lv_obj_t *ui_imgBedtimeNotification;
    extern lv_obj_t *ui_imgSpiderman;
    extern lv_obj_t *ui_imgSpidermanSleep;
    extern lv_obj_t *ui_pnlMain;
    extern lv_obj_t *ui_lblClock;
    extern lv_obj_t *ui_lblDate;
    extern lv_obj_t *ui_Rotate;
    extern lv_obj_t *ui_Label1;
    extern lv_obj_t *ui_sun;          // Added
    extern lv_obj_t *ui_moon;         // Added
    extern lv_obj_t *ui_lblWakeTime;  // Added
    extern lv_obj_t *ui_lblSleepTime; // Added
    extern lv_obj_t *ui____initial_actions0;

    void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif