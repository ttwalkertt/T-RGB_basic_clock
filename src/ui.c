#include "ui.h"
#include "clock_face_475.h"
#include "serial_wrapper.h"
#include <math.h>
#include <time.h>



int screen_heigth;
int screen_width;

lv_obj_t * ui_S1Main;
lv_obj_t * container;
lv_obj_t * hour_hand;
lv_obj_t * minute_hand;
lv_obj_t * second_hand;
lv_obj_t * led1;
lv_point_t hour_hands_points[2];
lv_point_t minute_hand_points[2];
lv_point_t second_hand_points[2];
int hour_hand_length = 120;
int minute_hand_length = 175;
int second_hand_length = 210;
///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// SCREENS ////////////////////
void ui_S1Main_screen_init(void)
{
    ui_S1Main = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_S1Main, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t * face_img = lv_img_create(ui_S1Main);
    lv_img_set_src(face_img, &clock_face_475);

    // the hands
    hour_hand = lv_line_create(ui_S1Main);
    minute_hand = lv_line_create(ui_S1Main);
    second_hand = lv_line_create(ui_S1Main);
    
    led1  = lv_led_create(ui_S1Main);
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_GREEN));
    lv_led_on(led1);

    lv_obj_align(face_img, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_align(led1, LV_ALIGN_CENTER, 0, 0);

    /* Create clock hands initial endpoints */
    
    minute_hand_points[0].x = 240;
    minute_hand_points[0].y = 240;
    minute_hand_points[1].x = -100;
    minute_hand_points[1].y = -100;

    hour_hands_points[0].x = 240;
    hour_hands_points[0].y = 240;
    hour_hands_points[1].x = 200;
    hour_hands_points[1].y = 200;

    second_hand_points[0].x = 240;
    second_hand_points[0].y = 240;
    second_hand_points[1].x = -200;
    second_hand_points[1].y = -200;

    /*Create clcok hand style*/
    static lv_style_t style_line_hr;
    lv_style_init(&style_line_hr);
    lv_style_set_line_width(&style_line_hr, 20);
    lv_style_set_line_color(&style_line_hr, lv_palette_main(LV_PALETTE_LIME));
    lv_style_set_line_rounded(&style_line_hr, true);
    lv_obj_add_style(hour_hand, &style_line_hr, 0);

    static lv_style_t style_line_mn;
    lv_style_init(&style_line_mn);
    lv_style_set_line_width(&style_line_mn, 10);
    lv_style_set_line_color(&style_line_mn, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line_mn, true);
    lv_obj_add_style(minute_hand, &style_line_mn, 0);

    static lv_style_t style_line_sc;
    lv_style_init(&style_line_sc);
    lv_style_set_line_width(&style_line_sc, 5);
    lv_style_set_line_color(&style_line_sc, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_line_sc, false);
    lv_obj_add_style(second_hand, &style_line_sc, 0);

    lv_line_set_points(hour_hand, hour_hands_points, 2);  // Set the points
    lv_line_set_points(minute_hand, minute_hand_points, 2);  // Set the points
    lv_line_set_points(second_hand, second_hand_points, 2);  // Set the points

    // Print the screen size if needed for bringup
    char buf[64];
    screen_heigth = lv_disp_get_hor_res(NULL);
    screen_width = lv_disp_get_ver_res(NULL);
    sniprintf(buf, sizeof(buf), "Screen: %d x %d\n", screen_heigth, screen_width);
    serial_print(buf);

}

// Function to plot a vector with length 'length' and angle 'angle' from the center of the screen
// The angle is in degrees and is converted to radians
// angle = 0 is up (12 o'clock) and positive angles rotate clockwise
// The vector is plotted on the screen at the end points
void calculate_line_endpoints(lv_point_t line_points[2], float length, float angle_degrees) {
    // Convert angle from degrees to radians and adjust so 0 degrees is up
    // Multiply by -1 to make positive angles rotate clockwise
    float angle_radians = (-angle_degrees + 90) * M_PI / 180.0;

    // Calculate end points
    int x_end = screen_width/2 + length * cos(angle_radians);
    int y_end = screen_heigth/2 - length * sin(angle_radians);

    // Update the start and end points
    // the start points are always the center of the screen
    line_points[0].x = screen_width/2;
    line_points[0].y = screen_heigth/2;
    line_points[1].x = x_end;
    line_points[1].y = y_end;

    char buf[64];
    if (VERBOSE > 1) {
        sniprintf(buf, sizeof(buf), "Endpoints: %d,%d %d,%d\n", line_points[0].x, line_points[0].y, line_points[1].x, line_points[1].y);
        serial_print(buf); 
        }

}


void set_hands(time_t now)
{
    // calculate the angle of the hour hand
    struct tm * ptm;
    ptm = localtime(&now); /* Get the local time */

    int hour = ptm->tm_hour;
    int minute = ptm->tm_min;
    int second = ptm->tm_sec;
    char buf[64];
    sniprintf(buf, sizeof(buf), "Time: %d:%d:%d\n", hour, minute, second);
    if (VERBOSE > 1) { serial_print(buf); }

    int angle_degrees = (hour * 360) / 12 + (minute * 360) / (12 * 60);
    calculate_line_endpoints(hour_hands_points, hour_hand_length, angle_degrees);

    // calculate the angle of the minute hand
    angle_degrees = (minute * 360) / 60;
    calculate_line_endpoints(minute_hand_points, minute_hand_length, angle_degrees);

    // calculate the angle of the second hand
    angle_degrees = (second * 360) / 60;
    calculate_line_endpoints(second_hand_points, second_hand_length, angle_degrees);

// debug and bringup code
    if (VERBOSE > 1) {
            sniprintf(buf, sizeof(buf), "hour endpoints: %d,%d %d,%d\n", 
                hour_hands_points[0].x, hour_hands_points[0].y, hour_hands_points[1].x, hour_hands_points[1].y);
            serial_print(buf);
            sniprintf(buf, sizeof(buf), "min endpoints: %d,%d %d,%d\n",
                minute_hand_points[0].x, minute_hand_points[0].y, minute_hand_points[1].x, minute_hand_points[1].y);
            serial_print(buf); 
            sniprintf(buf, sizeof(buf), "sec endpoints: %d,%d %d,%d\n",
                second_hand_points[0].x, second_hand_points[0].y, second_hand_points[1].x, second_hand_points[1].y);
            serial_print(buf); 
    }
    // update hands' positions
    lv_line_set_points(hour_hand, hour_hands_points, 2);  // Set the points
    lv_line_set_points(minute_hand, minute_hand_points, 2);  // Set the points
    lv_line_set_points(second_hand, second_hand_points, 2);  // Set the points
    
    lv_led_toggle(led1);
    
    lv_task_handler();
    
}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_S1Main_screen_init();
    lv_disp_load_scr(ui_S1Main);
    // Get the display size
    lv_disp_t * disp = lv_disp_get_default();
   

}