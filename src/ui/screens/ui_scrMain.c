// src/ui/screens/ui_scrMain.c
#include "../ui.h"

static void wake_time_plus_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_user_data(btn);
    int hour, minute;
    sscanf(lv_label_get_text(label), "%d:%d", &hour, &minute);

    if (minute == 0)
    {
        minute = 30;
    }
    else
    {
        minute = 0;
        hour = (hour + 1) % 24;
    }

    char buf[32];
    snprintf(buf, sizeof(buf), "%02d:%02d", hour, minute);
    lv_label_set_text(label, buf);
}

static void wake_time_minus_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_user_data(btn);
    int hour, minute;
    sscanf(lv_label_get_text(label), "%d:%d", &hour, &minute);

    if (minute == 30)
    {
        minute = 0;
    }
    else
    {
        minute = 30;
        hour = (hour - 1 + 24) % 24;
    }

    char buf[32];
    snprintf(buf, sizeof(buf), "%02d:%02d", hour, minute);
    lv_label_set_text(label, buf);
}

void ui_scrMain_screen_init(void)
{
    ui_scrMain = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_scrMain, LV_OBJ_FLAG_SCROLLABLE);


    // In ui_scrMain_screen_init()
    ui_imgBedtimeNotification = lv_img_create(ui_scrMain);
    lv_obj_set_size(ui_imgBedtimeNotification, 64, 64); // Adjust size as needed
    lv_obj_align(ui_imgBedtimeNotification, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_flag(ui_imgBedtimeNotification, LV_OBJ_FLAG_HIDDEN); // Hidden by default
    // Set your image source here
    lv_img_set_src(ui_imgBedtimeNotification, &spiderman); // Replace with your .c image name

    // Create main panel with background color
    ui_pnlMain = lv_obj_create(ui_scrMain);
    lv_obj_set_size(ui_pnlMain, lv_pct(100), lv_pct(100));
    lv_obj_center(ui_pnlMain);
 
    lv_obj_clear_flag(ui_pnlMain, LV_OBJ_FLAG_SCROLLABLE);

    // Create wake time controls group with rounded corners and shadow
    lv_obj_t *wake_group = lv_obj_create(ui_pnlMain);
    lv_obj_set_size(wake_group, 180, 120); // Made larger
    lv_obj_align(wake_group, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_set_style_bg_color(wake_group, lv_color_hex(0x535353), LV_PART_MAIN | LV_STATE_DEFAULT); // Darker blue
    lv_obj_set_style_bg_opa(wake_group, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(wake_group, 15, 0); // Rounded corners
    lv_obj_set_style_shadow_width(wake_group, 20, 0);
    lv_obj_set_style_shadow_color(wake_group, lv_color_hex(0x000000), 0);
    lv_obj_set_style_shadow_opa(wake_group, LV_OPA_30, 0);
    lv_obj_set_style_pad_all(wake_group, 15, 0); // More padding
    lv_obj_clear_flag(wake_group, LV_OBJ_FLAG_SCROLLABLE);

    // Wake time label with better styling
    lv_obj_t *wake_label = lv_label_create(wake_group);
    lv_label_set_text(wake_label, "Wake Up Time");
    lv_obj_align(wake_label, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_color(wake_label, lv_color_hex(0xECF0F1), 0); // Light blue
    lv_obj_set_style_text_font(wake_label, &lv_font_montserrat_14, 0);

    // Wake time value with prominent display
    ui_lblWakeTime = lv_label_create(wake_group);
    lv_label_set_text(ui_lblWakeTime, "06:00");
    lv_obj_align(ui_lblWakeTime, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_text_font(ui_lblWakeTime, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(ui_lblWakeTime, lv_color_hex(0xECF0F1), 0); // Light gray

    // Wake time buttons with better styling
    lv_obj_t *wake_minus = lv_btn_create(wake_group);
    lv_obj_set_size(wake_minus, 40, 40);
    lv_obj_align(wake_minus, LV_ALIGN_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_bg_color(wake_minus, lv_color_hex(0xE74C3C), 0); // Red
    lv_obj_set_style_radius(wake_minus, 20, 0);                       // Circular
    lv_obj_t *wake_minus_label = lv_label_create(wake_minus);
    lv_label_set_text(wake_minus_label, "-");
    lv_obj_center(wake_minus_label);
    lv_obj_set_user_data(wake_minus, ui_lblWakeTime);
    lv_obj_add_event_cb(wake_minus, wake_time_minus_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *wake_plus = lv_btn_create(wake_group);
    lv_obj_set_size(wake_plus, 40, 40);
    lv_obj_align(wake_plus, LV_ALIGN_BOTTOM_RIGHT, 0, 10);
    lv_obj_set_style_bg_color(wake_plus, lv_color_hex(0x2ECC71), 0); // Green
    lv_obj_set_style_radius(wake_plus, 20, 0);                       // Circular
    lv_obj_t *wake_plus_label = lv_label_create(wake_plus);
    lv_label_set_text(wake_plus_label, "+");
    lv_obj_center(wake_plus_label);
    lv_obj_set_user_data(wake_plus, ui_lblWakeTime);
    lv_obj_add_event_cb(wake_plus, wake_time_plus_event_cb, LV_EVENT_CLICKED, NULL);

    // Sleep time controls - mirror of wake time
    lv_obj_t *sleep_group = lv_obj_create(ui_pnlMain);
    lv_obj_set_size(sleep_group, 180, 120);
    lv_obj_align(sleep_group, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_set_style_bg_color(sleep_group, lv_color_hex(0x535353), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sleep_group, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(sleep_group, 15, 0);
    lv_obj_set_style_shadow_width(sleep_group, 20, 0);
    lv_obj_set_style_shadow_color(sleep_group, lv_color_hex(0x000000), 0);
    lv_obj_set_style_shadow_opa(sleep_group, LV_OPA_30, 0);
    lv_obj_set_style_pad_all(sleep_group, 15, 0);
    lv_obj_clear_flag(sleep_group, LV_OBJ_FLAG_SCROLLABLE);

    // Sleep time label
    lv_obj_t *sleep_label = lv_label_create(sleep_group);
    lv_label_set_text(sleep_label, "Sleep Time");
    lv_obj_align(sleep_label, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_color(sleep_label, lv_color_hex(0xECF0F1), 0); // Purple
    lv_obj_set_style_text_font(sleep_label, &lv_font_montserrat_14, 0);

    // Sleep time value
    ui_lblSleepTime = lv_label_create(sleep_group);
    lv_label_set_text(ui_lblSleepTime, "19:00");
    lv_obj_align(ui_lblSleepTime, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_text_font(ui_lblSleepTime, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(ui_lblSleepTime, lv_color_hex(0xECF0F1), 0);

    // Sleep time buttons
    lv_obj_t *sleep_minus = lv_btn_create(sleep_group);
    lv_obj_set_size(sleep_minus, 40, 40);
    lv_obj_align(sleep_minus, LV_ALIGN_BOTTOM_LEFT, 0, 10);
    lv_obj_set_style_bg_color(sleep_minus, lv_color_hex(0xE74C3C), 0);
    lv_obj_set_style_radius(sleep_minus, 20, 0);
    lv_obj_t *sleep_minus_label = lv_label_create(sleep_minus);
    lv_label_set_text(sleep_minus_label, "-");
    lv_obj_center(sleep_minus_label);
    lv_obj_set_user_data(sleep_minus, ui_lblSleepTime);
    lv_obj_add_event_cb(sleep_minus, wake_time_minus_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *sleep_plus = lv_btn_create(sleep_group);
    lv_obj_set_size(sleep_plus, 40, 40);
    lv_obj_align(sleep_plus, LV_ALIGN_BOTTOM_RIGHT, 0, 10);
    lv_obj_set_style_bg_color(sleep_plus, lv_color_hex(0x2ECC71), 0);
    lv_obj_set_style_radius(sleep_plus, 20, 0);
    lv_obj_t *sleep_plus_label = lv_label_create(sleep_plus);
    lv_label_set_text(sleep_plus_label, "+");
    lv_obj_center(sleep_plus_label);
    lv_obj_set_user_data(sleep_plus, ui_lblSleepTime);
    lv_obj_add_event_cb(sleep_plus, wake_time_plus_event_cb, LV_EVENT_CLICKED, NULL);

    // Create a semi-transparent panel for the clock
    lv_obj_t *clock_panel = lv_obj_create(ui_pnlMain);
    lv_obj_set_size(clock_panel, 550, 250);
    lv_obj_align(clock_panel, LV_ALIGN_CENTER, 0, 55);
    lv_obj_set_style_bg_color(clock_panel, lv_color_hex(0x34495E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(clock_panel, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(clock_panel, 20, 0);
    lv_obj_set_style_shadow_width(clock_panel, 30, 0);
    lv_obj_set_style_shadow_color(clock_panel, lv_color_hex(0x000000), 0);
    lv_obj_set_style_shadow_opa(clock_panel, LV_OPA_30, 0);
    lv_obj_clear_flag(clock_panel, LV_OBJ_FLAG_SCROLLABLE);

    // Digital clock display with shadow effect
    ui_lblClock = lv_label_create(clock_panel);
    lv_obj_set_style_text_font(ui_lblClock, &lv_font_montserrat_120, 0);
    lv_obj_set_style_text_color(ui_lblClock, lv_color_hex(0xECF0F1), 0);
    lv_label_set_text(ui_lblClock, "00:00:00");
    lv_obj_align(ui_lblClock, LV_ALIGN_TOP_MID, 0, 10);

    // Date display
    ui_lblDate = lv_label_create(clock_panel);
    lv_obj_set_style_text_font(ui_lblDate, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(ui_lblDate, lv_color_hex(0xBDC3C7), 0);
    lv_label_set_text(ui_lblDate, "Loading date...");
    lv_obj_align(ui_lblDate, LV_ALIGN_BOTTOM_MID, 0, -10);
}
