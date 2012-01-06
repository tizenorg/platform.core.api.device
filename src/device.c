/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */



#define LOG_TAG "TIZEN_SYSTEM_DEVICE"

#include <string.h>
#include <stdio.h>
#include <devman.h>
#include <device.h>
#include <errno.h>
#include <dlog.h>
#include <vconf.h>

#define _MSG_DEVICE_ERROR_INVALID_PARAMETER "Invalid parameter"
#define _MSG_DEVICE_ERROR_OPERATION_FAILED "Operation failed"

#define RETURN_ERR_MSG(err_code, msg) \
    do { \
        LOGE("[%s] "_MSG_##err_code"(0x%08x) : %s", __FUNCTION__, err_code, msg); \
        return err_code; \
    }while(0)

#define RETURN_ERR(err_code) \
    do { \
        LOGE("[%s] "_MSG_##err_code"(0x%08x)", __FUNCTION__, err_code); \
        return err_code; \
    }while(0)

static int _display[] = {
    DEV_DISPLAY_0,
    DEV_DISPLAY_1,
};

int device_get_display_numbers(int* device_number)
{
    if(device_number == NULL)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

    *device_number = device_get_display_count();
    if(*device_number < 0)
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);

    return DEVICE_ERROR_NONE;
}

int device_battery_get_percent( int* percent )
{
    if (percent == NULL) RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

	int pct = device_get_battery_pct();
	if(pct < 0) {
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }else{
		*percent = pct;
    }
	return DEVICE_ERROR_NONE;
}

int device_battery_is_full( bool* full )
{
    if (full == NULL) RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

    int f = device_get_battery_pct();
	if(f < 0) {
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }
	else
		*full = f == 100 ? true : false;

	return DEVICE_ERROR_NONE;
}

int device_get_brightness(int disp_idx, int* value)
{
	int val, disp, max_id;

    if(value == NULL) RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

    if(device_get_display_numbers(&max_id) < 0)
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);

    if(disp_idx < 0 || disp_idx >= max_id)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

    disp = _display[disp_idx];

    if(value == NULL)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

	val = device_get_display_brt(disp);
	
	if(val < 0) {
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }
	else
		*value = val;

	return DEVICE_ERROR_NONE;
}

int device_set_brightness(int disp_idx, int new_value)
{
    int max_value, val;
	int disp, max_id;
    
    if(device_get_display_numbers(&max_id) < 0)
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);

    if(disp_idx < 0 || disp_idx >= max_id)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

    disp = _display[disp_idx];

    if(new_value < 0)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

    device_get_max_brightness(disp, &max_value);

    if(new_value > max_value)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

	val = device_set_display_brt(disp, new_value);
	if(val < 0) {
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }
	return DEVICE_ERROR_NONE;
}

int device_get_max_brightness(int disp_idx, int* max_value)
{
	int val, disp, max_id;
    
    if(device_get_display_numbers(&max_id) < 0)
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);

    if(disp_idx < 0 || disp_idx >= max_id)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);
	
    disp = _display[disp_idx];

    if(max_value == NULL)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

	val = device_get_max_brt(disp);
	
	if(val < 0) {
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }
	else
		*max_value = val;

	return DEVICE_ERROR_NONE;
}

int device_battery_is_charging(bool *charging)
{
    // VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW
    int value, err;
    
    if(charging == NULL){
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);
    }

    err  = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, &value);

    if(err <0){
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }
    if(value == 1){
        *charging = true;
    }else if(value == 0){
        *charging = false;
    }else{
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }
    return DEVICE_ERROR_NONE;
}

static device_battery_cb changed_callback = NULL;
static void* changed_callback_user_data = NULL;

static void battery_changed_inside_cb(keynode_t* key, void* user_data)
{
    char* keyname = vconf_keynode_get_name(key);

    if(keyname != NULL && changed_callback != NULL && strcmp(keyname, VCONFKEY_SYSMAN_BATTERY_CAPACITY) == 0){
        int percent = 0;
        if(vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CAPACITY, &percent) == 0){
            changed_callback(percent, changed_callback_user_data);
        }
    }
}

int device_battery_set_cb(device_battery_cb callback, void* user_data)
{
    // VCONFKEY_SYSMAN_BATTERY_CAPACITY
    int err;
    if(callback == NULL)
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);

    changed_callback = callback;
    changed_callback_user_data = user_data;

    err = vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_CAPACITY, battery_changed_inside_cb, NULL);
    if(err < 0){
        RETURN_ERR(DEVICE_ERROR_INVALID_PARAMETER);
    }

    return DEVICE_ERROR_NONE;
}
int device_battery_unset_cb(void)
{
    int err = vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_CAPACITY, battery_changed_inside_cb);
    if(err < 0){
        RETURN_ERR(DEVICE_ERROR_OPERATION_FAILED);
    }
    changed_callback = NULL;
    changed_callback_user_data = NULL;

    return DEVICE_ERROR_NONE;
}
