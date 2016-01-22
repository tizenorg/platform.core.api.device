/*
 * Copyright (c) 2011 - 2015 Samsung Electronics Co., Ltd All Rights Reserved
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


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <tracker.h>

#include "power.h"
#include "display.h"
#include "common.h"
#include "dbus.h"

/**
 * Parameters for device_power_request_lock()
 */
#define STAY_CUR_STATE  0x1
#define GOTO_STATE_NOW  0x2
#define HOLD_KEY_BLOCK  0x4
#define STANDBY_MODE    0x8

/**
 * Parameters for device_power_request_unlock()
 */
#define PM_SLEEP_MARGIN 0x0 /**< keep guard time for unlock */
#define PM_RESET_TIMER  0x1 /**< reset timer for unlock */
#define PM_KEEP_TIMER   0x2 /**< keep timer for unlock */

#define METHOD_LOCK_STATE       "lockstate"
#define METHOD_UNLOCK_STATE     "unlockstate"
#define METHOD_CHANGE_STATE     "changestate"
#define METHOD_REBOOT           "Reboot"

#define STR_STAYCURSTATE "staycurstate"
#define STR_GOTOSTATENOW "gotostatenow"

#define STR_HOLDKEYBLOCK "holdkeyblock"
#define STR_STANDBYMODE  "standbymode"
#define STR_NULL         "NULL"

#define STR_SLEEP_MARGIN "sleepmargin"
#define STR_RESET_TIMER  "resettimer"
#define STR_KEEP_TIMER   "keeptimer"

#define STR_LCD_OFF   "lcdoff"
#define STR_LCD_DIM   "lcddim"
#define STR_LCD_ON    "lcdon"

#define LOCK_CPU_TIMEOUT_MAX       60000 /* milliseconds */

static guint off_lock_timeout;

static char *get_state_str(display_state_e state)
{
	switch (state) {
	case DISPLAY_STATE_NORMAL:
		return STR_LCD_ON;
	case DISPLAY_STATE_SCREEN_DIM:
		return STR_LCD_DIM;
	case DISPLAY_STATE_SCREEN_OFF:
		return STR_LCD_OFF;
	default:
		break;
	}
	return NULL;
}

static void remove_off_lock_timeout(void)
{
	_I("Power lock timeout handler removed");
	if (off_lock_timeout) {
		g_source_remove(off_lock_timeout);
		off_lock_timeout = 0;
	}
}

static gboolean off_lock_timeout_expired(gpointer data)
{
	pid_t pid;
	int ret, ref;

	_I("Power lock timeout expired");

	ret = tracker_get_power_lock_ref(&ref);
	if (ret != TRACKER_ERROR_NONE) {
		_E("Failed to get reference count of power lock");
		goto out;
	}

	_I("reference count of power lock is (%d)", ref);
	if (ref > 0)
		return G_SOURCE_CONTINUE;

out:
	ret = device_power_release_lock(POWER_LOCK_CPU);
	if (ret != DEVICE_ERROR_NONE)
		_E("Failed to lock power(CPU) again(%d)", ret);

	return G_SOURCE_REMOVE;
}

static void add_off_lock_timeout(void)
{
	guint id;

	remove_off_lock_timeout();

	id = g_timeout_add(LOCK_CPU_TIMEOUT_MAX,
			off_lock_timeout_expired, NULL);
	if (id)
		off_lock_timeout = id;
}

static void lock_cb(void *data, GVariant *result, GError *err)
{
	int ret;

	if (!result) {
		_E("no message : %s", err->message);
		return;
	}

	g_variant_get(result, "(i)", &ret);
	_D("%s-%s : %d", DEVICED_INTERFACE_DISPLAY, METHOD_LOCK_STATE, ret);

	if (ret < 0)
		remove_off_lock_timeout();
}

static int lock_state(display_state_e state, unsigned int flag, int timeout_ms)
{
	char *arr[4];
	char str_timeout[32];

	arr[0] = get_state_str(state);
	if (!arr[0])
		return -EINVAL;

	if (flag & GOTO_STATE_NOW)
		arr[1] = STR_GOTOSTATENOW;
	else
		arr[1] = STR_STAYCURSTATE;

	if (flag & HOLD_KEY_BLOCK)
		arr[2] = STR_HOLDKEYBLOCK;
	else if (flag & STANDBY_MODE)
		arr[2] = STR_STANDBYMODE;
	else
		arr[2] = STR_NULL;

	snprintf(str_timeout, sizeof(str_timeout), "%d", timeout_ms);
	arr[3] = str_timeout;

	return dbus_method_async_with_reply(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_LOCK_STATE, "sssi", arr, lock_cb, -1, NULL);
}

static void unlock_cb(void *data, GVariant *result, GError *err)
{
	int ret;

	if (!result) {
		_E("no message : %s", err->message);
		return;
	}

	g_variant_get(result, "(i)", &ret);
	_D("%s-%s : %d", DEVICED_INTERFACE_DISPLAY, METHOD_UNLOCK_STATE, ret);
}

static int unlock_state(display_state_e state, unsigned int flag)
{
	char *arr[2];

	arr[0] = get_state_str(state);
	if (!arr[0])
		return -EINVAL;

	if (flag == PM_SLEEP_MARGIN)
		arr[1] = STR_SLEEP_MARGIN;
	else if (flag == PM_RESET_TIMER)
		arr[1] = STR_RESET_TIMER;
	else if (flag == PM_KEEP_TIMER)
		arr[1] = STR_KEEP_TIMER;
	else
		return -EINVAL;

	return dbus_method_async_with_reply(DEVICED_BUS_NAME,
			DEVICED_PATH_DISPLAY, DEVICED_INTERFACE_DISPLAY,
			METHOD_UNLOCK_STATE, "ss", arr, unlock_cb, -1, NULL);
}

int device_power_request_lock(power_lock_e type, int timeout_ms)
{
	int ret;

	if (timeout_ms < 0)
		return DEVICE_ERROR_INVALID_PARAMETER;

	if (type == POWER_LOCK_CPU) {
		ret = lock_state(DISPLAY_STATE_SCREEN_OFF, STAY_CUR_STATE, timeout_ms);
		if (ret == 0 &&
			(timeout_ms == 0 || timeout_ms > LOCK_CPU_TIMEOUT_MAX))
			add_off_lock_timeout();
	} else if (type == POWER_LOCK_DISPLAY)
		ret = lock_state(DISPLAY_STATE_NORMAL, STAY_CUR_STATE, timeout_ms);
	else if (type == POWER_LOCK_DISPLAY_DIM)
		ret = lock_state(DISPLAY_STATE_SCREEN_DIM, STAY_CUR_STATE, timeout_ms);
	else
		return DEVICE_ERROR_INVALID_PARAMETER;

	return errno_to_device_error(ret);
}

int device_power_release_lock(power_lock_e type)
{
	int ret;

	if (type == POWER_LOCK_CPU) {
		ret = unlock_state(DISPLAY_STATE_SCREEN_OFF, PM_SLEEP_MARGIN);
		if (ret == 0 && off_lock_timeout > 0)
			remove_off_lock_timeout();
	} else if (type == POWER_LOCK_DISPLAY)
		ret = unlock_state(DISPLAY_STATE_NORMAL, PM_KEEP_TIMER);
	else if (type == POWER_LOCK_DISPLAY_DIM)
		ret = unlock_state(DISPLAY_STATE_SCREEN_DIM, PM_KEEP_TIMER);
	else
		return DEVICE_ERROR_INVALID_PARAMETER;

	return errno_to_device_error(ret);
}

int device_power_wakeup(bool dim)
{
	if (dim)
		return device_display_change_state(DISPLAY_STATE_SCREEN_DIM);

	return device_display_change_state(DISPLAY_STATE_NORMAL);
}

int device_power_reboot(const char *reason)
{
	char *arr[1];
	int ret;

	if (!reason)
		return DEVICE_ERROR_INVALID_PARAMETER;

	arr[0] = (char *)reason;
	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_POWER,
			DEVICED_INTERFACE_POWER,
			METHOD_REBOOT, "s", arr);
	return errno_to_device_error(ret);
}
