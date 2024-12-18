// spidermansleep.h

#ifndef SPIDERMANSLEEP_H
#define SPIDERMANSLEEP_H

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl.h" 
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMAGE_SPIDERMANSLEEP
#define LV_ATTRIBUTE_IMAGE_SPIDERMANSLEEP
#endif

extern const lv_image_dsc_t spidermansleep;

#endif /*SPIDERMANSLEEP_H*/