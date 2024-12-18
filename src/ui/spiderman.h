// spiderman.h

#ifndef SPIDERMAN_H
#define SPIDERMAN_H

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

#ifndef LV_ATTRIBUTE_IMAGE_SPIDERMAN
#define LV_ATTRIBUTE_IMAGE_SPIDERMAN
#endif

extern const lv_image_dsc_t spiderman;

#endif /*SPIDERMAN_H*/