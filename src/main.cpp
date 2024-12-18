#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>
#include <time.h>
#include <WiFi.h>
#include "config.h"

// Forward declarations
static void cleanup_animation(lv_anim_t *anim);
static void update_background(bool isDay);
static lv_anim_t fade_animation;  // Static animation object
// ============= Global Variables and Constants =============
// Animation and UI elements
static lv_obj_t *spiderman_img = NULL;
static lv_obj_t *spidermansleep_img = NULL;

// Style management
static lv_style_t style_bg_day;
static lv_style_t style_bg_night;
static lv_style_t style_pattern;
static bool styles_initialized = false;
static bool pattern_style_initialized = false;

// Display update timing
static ulong next_clock_update;
static ulong lv_last_tick;

// Constants for time management
static constexpr unsigned long WIFI_TIMEOUT_MS = 20000;              // 20 seconds
static constexpr unsigned long TIME_SYNC_TIMEOUT_MS = 10000;         // 10 seconds
static constexpr float BEDTIME_WINDOW_HOURS = 1.0f + (1.0f / 60.0f); // 1 hour and 1 minute

// Logging tag for consistent log messages
static const char *TAG = "SmartDisplay";

// ============= Style Management =============
static void init_styles()
{
    if (!styles_initialized)
    {
        // Initialize day style
        lv_style_init(&style_bg_day);
        lv_style_set_bg_color(&style_bg_day, lv_color_hex(0xFFB74D));      // Light Orange base
        lv_style_set_bg_grad_color(&style_bg_day, lv_color_hex(0xF1C40F)); // Yellow gradient
        lv_style_set_bg_grad_dir(&style_bg_day, LV_GRAD_DIR_VER);
        lv_style_set_bg_opa(&style_bg_day, LV_OPA_COVER);
        lv_style_set_bg_grad_stop(&style_bg_day, 128);

        // Initialize night style
        lv_style_init(&style_bg_night);
        lv_style_set_bg_color(&style_bg_night, lv_color_hex(0x3498DB));      // Light Blue base
        lv_style_set_bg_grad_color(&style_bg_night, lv_color_hex(0x2C3E50)); // Dark Blue gradient
        lv_style_set_bg_grad_dir(&style_bg_night, LV_GRAD_DIR_VER);
        lv_style_set_bg_opa(&style_bg_night, LV_OPA_COVER);
        lv_style_set_bg_grad_stop(&style_bg_night, 128);

        styles_initialized = true;
        log_i("%s: Gradient styles initialized", TAG);
    }
}

static void init_pattern_style()
{
    if (!pattern_style_initialized)
    {
        lv_style_init(&style_pattern);
        // Add any pattern-specific style properties here
        pattern_style_initialized = true;
        log_i("%s: Pattern style initialized", TAG);
    }
}



static lv_anim_t* create_fade_animation(lv_obj_t *obj, uint32_t start_value, uint32_t end_value, uint32_t duration) {
    lv_anim_init(&fade_animation);
    lv_anim_set_var(&fade_animation, obj);
    lv_anim_set_values(&fade_animation, start_value, end_value);
    lv_anim_set_time(&fade_animation, duration);
    lv_anim_set_exec_cb(&fade_animation, [](void *obj, int32_t v) {
        lv_obj_set_style_img_opa((lv_obj_t *)obj, v, 0);
    });
    
    return &fade_animation;
}

// ============= Spiderman Image Management =============
void showSpidermanPopup()
{
    if (spiderman_img == NULL)
    {
        spiderman_img = lv_img_create(ui_pnlMain);
        if (!spiderman_img)
        {
            log_e("%s: Failed to create spiderman image", TAG);
            return;
        }

        lv_img_set_src(spiderman_img, &spiderman);
        lv_obj_set_style_img_opa(spiderman_img, 0, 0);
        lv_obj_align(spiderman_img, LV_ALIGN_LEFT_MID, -20, 0);

        lv_anim_t *anim = create_fade_animation(spiderman_img, 0, 255, 500);
        if (anim)
        {
            lv_anim_start(anim);
        }
    }
}

void hideSpidermanPopup() {
    if (spiderman_img != NULL) {
        lv_anim_t *anim = create_fade_animation(spiderman_img, 255, 0, 500);
        if (anim) {
            lv_anim_set_ready_cb(anim, [](lv_anim_t *a) {
                if (a && a->var) {
                    lv_obj_del((lv_obj_t*)a->var);
                    spiderman_img = NULL;
                }
            });
            lv_anim_start(anim);
        }
    }
}

void showSpidermanSleepPopup()
{
    if (spidermansleep_img == NULL)
    {
        spidermansleep_img = lv_img_create(ui_pnlMain);
        if (!spidermansleep_img)
        {
            log_e("%s: Failed to create spiderman sleep image", TAG);
            return;
        }

        lv_img_set_src(spidermansleep_img, &spidermansleep);
        lv_obj_set_style_img_opa(spidermansleep_img, 0, 0);
        lv_obj_align(spidermansleep_img, LV_ALIGN_BOTTOM_MID, -10, 0);

        lv_anim_t *anim = create_fade_animation(spidermansleep_img, 0, 255, 500);
        if (anim)
        {
            lv_anim_start(anim);
        }
    }
}

void hideSpidermanSleepPopup() {
    if (spidermansleep_img != NULL) {
        lv_anim_t *anim = create_fade_animation(spidermansleep_img, 255, 0, 500);
        if (anim) {
            lv_anim_set_ready_cb(anim, [](lv_anim_t *a) {
                if (a && a->var) {
                    lv_obj_del((lv_obj_t*)a->var);
                    spidermansleep_img = NULL;
                }
            });
            lv_anim_start(anim);
        }
    }
}

// ============= Time Management =============
float timeStringToHours(const char *timeStr)
{
    int hours = 0, minutes = 0;
    if (sscanf(timeStr, "%d:%d", &hours, &minutes) != 2)
    {
        log_w("%s: Failed to parse time string: %s", TAG, timeStr);
        return 0.0f;
    }
    return hours + minutes / 60.0f;
}

bool isNearBedtime(float currentTime, float sleepTime)
{
    float timeUntilBed = sleepTime - currentTime;
    if (timeUntilBed < 0)
    {
        timeUntilBed += 24; // Handle wrap around midnight
    }
    return timeUntilBed <= BEDTIME_WINDOW_HOURS;
}

bool isAfterBedtime(float currentTime, float sleepTime)
{
    float timeSinceBed = currentTime - sleepTime;
    if (timeSinceBed < 0)
    {
        timeSinceBed += 24;
    }
    return timeSinceBed <= BEDTIME_WINDOW_HOURS;
}

// ============= Display Updates =============
static void create_gradient_background(lv_obj_t *obj, bool isDay)
{
    init_styles();

    lv_obj_remove_style(obj, &style_bg_day, 0);
    lv_obj_remove_style(obj, &style_bg_night, 0);

    lv_obj_add_style(obj, isDay ? &style_bg_day : &style_bg_night, 0);
    lv_obj_invalidate(obj);
}

void updateTimeDisplay(const struct tm &timeinfo)
{
    // Update time display
    static char timeStr[9];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d",
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);

    // Update date display
    static char dateStr[32];
    static const char *const MONTHS[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"};

    snprintf(dateStr, sizeof(dateStr), "%s %d, %d",
             MONTHS[timeinfo.tm_mon],
             timeinfo.tm_mday,
             timeinfo.tm_year + 1900);

    // Update UI elements
    lv_label_set_text(ui_lblClock, timeStr);
    lv_label_set_text(ui_lblDate, dateStr);

    // Calculate current time and check schedules
    float currentTime = timeinfo.tm_hour + timeinfo.tm_min / 60.0f;
    float wakeTime = timeStringToHours(lv_label_get_text(ui_lblWakeTime));
    float sleepTime = timeStringToHours(lv_label_get_text(ui_lblSleepTime));

    // Handle Spiderman animations
    if (isNearBedtime(currentTime, sleepTime))
    {
        showSpidermanPopup();
    }
    else
    {
        hideSpidermanPopup();
    }

    if (isAfterBedtime(currentTime, sleepTime))
    {
        showSpidermanSleepPopup();
    }
    else
    {
        hideSpidermanSleepPopup();
    }

    // Update background based on wake/sleep schedule
    bool isWakeTime;
    if (sleepTime > wakeTime)
    {
        isWakeTime = (currentTime >= wakeTime && currentTime < sleepTime);
    }
    else
    {
        isWakeTime = !(currentTime >= sleepTime && currentTime < wakeTime);
    }

    create_gradient_background(ui_pnlMain, isWakeTime);

// Update RGB LED if available
#ifdef BOARD_HAS_RGB_LED
    smartdisplay_led_set_rgb(isWakeTime ? 1 : 0, isWakeTime ? 1 : 0, !isWakeTime);
#endif
}

void updateClock()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    if (!localtime_r(&now, &timeinfo))
    {
        log_e("%s: Failed to get local time", TAG);
        return;
    }
    updateTimeDisplay(timeinfo);
}

// ============= Event Handlers =============
void OnRotateClicked(lv_event_t *e)
{
    auto disp = lv_disp_get_default();
    auto rotation = (lv_display_rotation_t)((lv_disp_get_rotation(disp) + 1) % (LV_DISPLAY_ROTATION_270 + 1));
    lv_display_set_rotation(disp, rotation);
    log_i("%s: Display rotation changed to %d", TAG, rotation);
}

// ============= Setup Functions =============
bool setupWiFi()
{
    log_i("%s: Connecting to WiFi SSID: %s", TAG, WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS)
    {
        delay(500);
        log_i("%s: Connecting to WiFi...", TAG);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        log_e("%s: WiFi connection failed", TAG);
        return false;
    }

    log_i("%s: WiFi connected, IP: %s", TAG, WiFi.localIP().toString().c_str());
    return true;
}

bool setupTime()
{
    log_i("%s: Configuring time", TAG);

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    setenv("TZ", SELECTED_TIMEZONE, 1);
    tzset();

    unsigned long startAttempt = millis();
    time_t now = time(nullptr);

    while (now < 24 * 3600 && millis() - startAttempt < TIME_SYNC_TIMEOUT_MS)
    {
        delay(100);
        now = time(nullptr);
    }

    if (now < 24 * 3600)
    {
        log_e("%s: Time sync failed", TAG);
        return false;
    }

    log_i("%s: Time synchronized", TAG);
    return true;
}

// ============= Main Program =============
// ============= Main Program =============
void setup()
{
#ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(5000);
#endif

    // Initialize serial communication
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    // Log system information
    log_i("=============== SYSTEM INFO ===============");
    log_i("Board: %s", BOARD_NAME);
    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)",
          ESP.getChipModel(),
          ESP.getChipRevision(),
          getCpuFrequencyMhz(),
          ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("Free PSRAM: %d bytes", ESP.getPsramSize());
    log_i("SDK version: %s", ESP.getSdkVersion());
    log_i("==========================================");

    // Initialize display and UI
    smartdisplay_init();
    auto disp = lv_disp_get_default();
    lv_disp_set_rotation(disp, LV_DISP_ROTATION_180); // Note the constant name change
    ui_init();
    init_styles();
    init_pattern_style();

    // Setup network and time
    if (!setupWiFi())
    {
        log_w("%s: Continuing without WiFi connection", TAG);
    }

    if (!setupTime())
    {
        log_w("%s: Continuing without time sync", TAG);
    }

    // Initialize timing variables for the main loop
    next_clock_update = millis() + 1000;
    lv_last_tick = millis();

    log_i("%s: Setup complete", TAG);
}

void loop()
{
    auto const now = millis();

    // Handle clock updates
    if (now >= next_clock_update)
    {
        next_clock_update = now + 1000;
        updateClock();
    }

    // Update LVGL timing
    lv_tick_inc(now - lv_last_tick);
    lv_last_tick = now;

    // Handle LVGL tasks
    lv_timer_handler();

    // Optional: Monitor and report system health periodically
    static unsigned long last_health_check = 0;
    if (now - last_health_check >= 300000)
    { // Every 5 minutes
        log_i("%s: Health Check - Free heap: %d bytes", TAG, ESP.getFreeHeap());
        last_health_check = now;
    }

    // Optional: Monitor WiFi connection and attempt reconnection if needed
    if (WiFi.status() != WL_CONNECTED)
    {
        static unsigned long last_reconnect_attempt = 0;
        if (now - last_reconnect_attempt >= 60000)
        { // Try every minute
            log_w("%s: WiFi disconnected, attempting reconnection", TAG);
            setupWiFi();
            last_reconnect_attempt = now;
        }
    }

    // Small delay to prevent watchdog issues
    delay(5);
}