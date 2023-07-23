#include "clk_face_1.h"

#pragma once

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

#include <time.h>

extern lv_obj_t * ui_S1Main;

void ui_init(void);
void set_hands(time_t now);

#ifdef __cplusplus
} /*extern "C"*/
#endif