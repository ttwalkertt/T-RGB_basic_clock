

#ifdef __cplusplus
extern "C" {
#endif
#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

extern const lv_img_dsc_t clock_face_475;

#ifdef __cplusplus
}
#endif