#include "ui.h"
#include "clock_face_475.h"
#include "serial_wrapper.h"
#include <math.h>
#include <time.h>


int screen_heigth;
int screen_width;

lv_obj_t * ui_S1Main;
lv_obj_t * container;
//lv_obj_t * ui_S1LabelTitle;
lv_obj_t * hour_hand;
lv_obj_t * minute_hand;
lv_obj_t * led1;
lv_point_t hour_line_points[2];
lv_point_t minute_line_points[2];

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
    hour_hand = lv_line_create(ui_S1Main);
    minute_hand = lv_line_create(ui_S1Main);
    led1  = lv_led_create(ui_S1Main);

    lv_obj_align(face_img, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_align(led1, LV_ALIGN_CENTER, 0, 0);

    /* Create clock hands */
    
    minute_line_points[0].x = 0;
    minute_line_points[0].y = 0;
    minute_line_points[1].x = -200;
    minute_line_points[1].y = -200;

    hour_line_points[0].x = 0;
    hour_line_points[0].y = 0;
    hour_line_points[1].x = 200;
    hour_line_points[1].y = 200;

    /*Create style*/
    static lv_style_t style_line_hr;
    lv_style_init(&style_line_hr);
    lv_style_set_line_width(&style_line_hr, 20);
    lv_style_set_line_color(&style_line_hr, lv_palette_main(LV_PALETTE_LIME));
    lv_style_set_line_rounded(&style_line_hr, true);
    lv_obj_add_style(hour_hand, &style_line_hr, 0);

    static lv_style_t style_line_mn;
    lv_style_init(&style_line_mn);
    lv_style_set_line_width(&style_line_mn, 10);
    lv_style_set_line_color(&style_line_mn, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_line_mn, true);
    lv_obj_add_style(minute_hand, &style_line_mn, 0);

    lv_line_set_points(hour_hand, hour_line_points, 2);  // Set the points
    lv_line_set_points(minute_hand, minute_line_points, 2);  // Set the points

    lv_led_on(led1);

    // Print the screen size
    char buf[64];
    screen_heigth = lv_disp_get_hor_res(NULL);
    screen_width = lv_disp_get_ver_res(NULL);
    sniprintf(buf, sizeof(buf), "Screen: %d x %d\n", screen_heigth, screen_width);
    serial_print(buf);
    // print the container size
    //sniprintf(buf, sizeof(buf), "Container: %d x %d\n", lv_obj_get_width(container), lv_obj_get_height(container));
    //serial_print(buf);
    // print the image size
    //sniprintf(buf, sizeof(buf), "Image: %d x %d\n", lv_obj_get_width(face_img), lv_obj_get_height(face_img));
    //serial_print(buf);

}

// Helper function to convert degrees to radians
static inline double deg_to_rad(double degrees) {
    return degrees * M_PI / 180.0;
}


// Function to plot a vector with length 'length' and angle 'angle' from the center of the screen
void calculate_line_endpoints(lv_point_t line_points[2], float length, float angle_degrees) {
    // Convert angle from degrees to radians and adjust so 0 degrees is up
    // Multiply by -1 to make positive angles rotate clockwise
    float angle_radians = (-angle_degrees - 90) * M_PI / 180.0;

    // Calculate end point
    int x_end = screen_width/2 + length * cos(angle_radians);
    int y_end = screen_heigth/2 - length * sin(angle_radians);

    // Define the start and end points
    line_points[0].x = screen_width/2;
    line_points[0].y = screen_heigth/2;
    line_points[1].x = x_end;
    line_points[1].y = y_end;
    char buf[64];
    sniprintf(buf, sizeof(buf), "Endpoints: %d,%d %d,%d\n", line_points[0].x, line_points[0].y, line_points[1].x, line_points[1].y);
    serial_print(buf);

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
    serial_print(buf);

    int angle_degrees = (hour * 360) / 12 + (minute * 360) / (12 * 60);
    calculate_line_endpoints(hour_line_points, 100, angle_degrees);

    // calculate the angle of the minute hand
    angle_degrees = (minute * 360) / 60;
    calculate_line_endpoints(minute_line_points, 200, angle_degrees);

    sniprintf(buf, sizeof(buf), "hour endpoints: %d,%d %d,%d\n", 
        hour_line_points[0].x, hour_line_points[0].y, hour_line_points[1].x, hour_line_points[1].y);
    serial_print(buf);
    sniprintf(buf, sizeof(buf), "min endpoints: %d,%d %d,%d\n",
        minute_line_points[0].x, minute_line_points[0].y, minute_line_points[1].x, minute_line_points[1].y);
    serial_print(buf);
    lv_line_set_points(hour_hand, hour_line_points, 2);  // Set the points
    lv_line_set_points(minute_hand, minute_line_points, 2);  // Set the points
    
    lv_led_toggle(led1);
    
    //lv_led_toggle(led1);
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