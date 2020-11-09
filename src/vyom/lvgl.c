/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
//  LVGL Interface for PineTime on Mynewt
#include <assert.h>
#include <os/os.h>
#include <hal/hal_bsp.h>
#include <hal/hal_gpio.h>
#include <hal/hal_system.h>
#include <hal/hal_flash.h>
#include <console/console.h>
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

static bool pinetime_lvgl_mynewt_started = false;

lv_group_t * group;
extern lv_indev_t * indev_encoder;

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        printf("Clicked\n");
    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

/// Init the LVGL library. Called by sysinit() during startup, defined in pkg.yml.
void pinetime_lvgl_mynewt_init(void) {    
    // console_printf("Init LVGL...\n"); console_flush();
    assert(pinetime_lvgl_mynewt_started == false);

    //  Init the display controller
    int rc = pinetime_lvgl_mynewt_init_display(); assert(rc == 0);

    //  Init the LVGL display
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    group = lv_group_create();
    lv_indev_set_group(indev_encoder, group);
    pinetime_lvgl_mynewt_started = true;
}

/// Render a Button Widget and a Label Widget
int pinetime_lvgl_mynewt_test(void) {
    // console_printf("Test LVGL widgets...\n"); console_flush();
    lv_obj_t *btn = lv_btn_create(lv_scr_act(), NULL);     //  Add a button the current screen
    lv_obj_set_pos(btn, 10, 10);                           //  Set its position
    lv_obj_set_size(btn, 120, 50);                         //  Set its size
    lv_obj_set_event_cb(btn, event_handler);

    lv_obj_t *label = lv_label_create(btn, NULL);          //  Add a label to the button
    lv_label_set_text(label, "Button");               //  Set the label text

    lv_obj_t *btn2 = lv_btn_create(lv_scr_act(), NULL);     //  Add a button the current screen
    lv_obj_set_pos(btn2, 10, 70);                           //  Set its position
    lv_obj_set_size(btn2, 120, 50); 
    lv_obj_set_event_cb(btn2, event_handler);

    lv_obj_t *label2 = lv_label_create(btn2, NULL);          //  Add a label to the button
    lv_label_set_text(label2, "Button2");               //  Set the label text

    lv_group_add_obj(group, btn);
    lv_group_add_obj(group, btn2);
    return 0;
}

/// Render the LVGL display
int pinetime_lvgl_mynewt_render(void) {
    // console_printf("Render LVGL display...\n"); console_flush();
    //  Must tick at least 100 milliseconds to force LVGL to update display
    lv_tick_inc(200);
    //  LVGL will flush our display driver
    lv_task_handler();
    return 0;
}
