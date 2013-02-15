/*
 * capi-system-device
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <tet_api.h>
#include <device.h>

#define API_NAME_DEVICE_BATTERY_GET_PERCENT "device_battery_get_percent"
#define API_NAME_DEVICE_BATTERY_IS_FULL "device_battery_is_full"
#define API_NAME_DEVICE_BATTERY_IS_CHARGING "device_battery_is_charging"
#define API_NAME_DEVICE_BATTERY_SET_CB "device_battery_set_cb"
#define API_NAME_DEVICE_BATTERY_UNSET_CB "device_battery_unset_cb"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_system_device_battery_get_percent_p(void);
static void utc_system_device_battery_get_percent_n(void);
static void utc_system_device_battery_is_full_p(void);
static void utc_system_device_battery_is_full_n(void);
static void utc_system_device_battery_is_charging_p(void);
static void utc_system_device_battery_is_charging_n(void);
static void utc_system_device_battery_set_cb_p(void);
static void utc_system_device_battery_set_cb_n(void);
static void utc_system_device_battery_unset_cb_p(void);


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_system_device_battery_get_percent_p, POSITIVE_TC_IDX },
	{ utc_system_device_battery_get_percent_n, NEGATIVE_TC_IDX },
	{ utc_system_device_battery_is_full_p, POSITIVE_TC_IDX },
	{ utc_system_device_battery_is_full_n, NEGATIVE_TC_IDX },
	{ utc_system_device_battery_is_charging_p, POSITIVE_TC_IDX },
	{ utc_system_device_battery_is_charging_n, NEGATIVE_TC_IDX },
	{ utc_system_device_battery_set_cb_p, POSITIVE_TC_IDX },
	{ utc_system_device_battery_set_cb_n, NEGATIVE_TC_IDX },
	{ utc_system_device_battery_unset_cb_p, POSITIVE_TC_IDX },
	{ NULL, 0},
};

static void startup(void)
{
	/* start of TC */
}

static void cleanup(void)
{
	/* end of TC */
}

/**
 * @brief Positive test case of device_battery_get_percent()
 */
static void utc_system_device_battery_get_percent_p(void)
{
    int percent = 0;
    int error = DEVICE_ERROR_NONE;
    error = device_battery_get_percent(&percent);

    dts_check_eq(API_NAME_DEVICE_BATTERY_GET_PERCENT, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_battery_get_percent()
 */
static void utc_system_device_battery_get_percent_n(void)
{
    int error = DEVICE_ERROR_NONE;
    error = device_battery_get_percent(NULL);

    dts_check_ne(API_NAME_DEVICE_BATTERY_GET_PERCENT, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Positive test case of device_battery_is_full()
 */
static void utc_system_device_battery_is_full_p(void)
{
    bool full;
    int error = DEVICE_ERROR_NONE;
    error = device_battery_is_full(&full);
    dts_check_eq(API_NAME_DEVICE_BATTERY_IS_FULL, error, DEVICE_ERROR_NONE);
}

/**
 * @brief Negative test case of device_battery_is_full()
 */
static void utc_system_device_battery_is_full_n(void)
{
    int error = DEVICE_ERROR_NONE;
    error = device_battery_is_full(NULL);
    dts_check_ne(API_NAME_DEVICE_BATTERY_IS_FULL, error, DEVICE_ERROR_NONE);
}


static void utc_system_device_battery_is_charging_p(void)
{
    bool charging;
    int error = DEVICE_ERROR_NONE;
    error = device_battery_is_charging(&charging);
    dts_check_eq(API_NAME_DEVICE_BATTERY_IS_CHARGING, error, DEVICE_ERROR_NONE);
}

static void utc_system_device_battery_is_charging_n(void)
{
    bool charging;
    int error = DEVICE_ERROR_NONE;
    error = device_battery_is_charging(NULL);
    dts_check_ne(API_NAME_DEVICE_BATTERY_IS_CHARGING, error, DEVICE_ERROR_NONE);
}

static void battery_cb(int percent, void *user_data)
{
}

static void utc_system_device_battery_set_cb_p(void)
{
    int error = device_battery_set_cb(battery_cb, NULL);
    device_battery_unset_cb();
    dts_check_eq(API_NAME_DEVICE_BATTERY_SET_CB, error, DEVICE_ERROR_NONE);
}

static void utc_system_device_battery_set_cb_n(void)
{
    int error = device_battery_set_cb(NULL, NULL);
    device_battery_unset_cb();
    dts_check_ne(API_NAME_DEVICE_BATTERY_SET_CB, error, DEVICE_ERROR_NONE);
}

static void utc_system_device_battery_unset_cb_p(void)
{
    device_battery_set_cb(battery_cb, NULL);
    int error = device_battery_unset_cb();
    dts_check_eq(API_NAME_DEVICE_BATTERY_SET_CB, error, DEVICE_ERROR_NONE);
}
