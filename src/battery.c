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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vconf.h>

#include "battery.h"
#include "common.h"
#include "dbus.h"

#define METHOD_GET_PERCENT		"GetPercent"
#define METHOD_GET_INFO         "GetBatteryInfo"

#define INFO_MAX 32

struct battery_info {
	char status[INFO_MAX];
	char health[INFO_MAX];
	char power_source[INFO_MAX];
	int online;
	int present;
	int capacity;
	int current_now;
	int current_average;
};

int device_battery_get_percent(int *percent)
{
	int ret;

	if (!percent)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = dbus_method_sync(DEVICED_BUS_NAME,
			DEVICED_PATH_BATTERY, DEVICED_INTERFACE_BATTERY,
			METHOD_GET_PERCENT, NULL, NULL);
	/* regard not suppoted as disconnected */
	if (ret == -ENOTSUP)
		ret = 0;
	else if (ret < 0)
		return errno_to_device_error(ret);

	*percent = ret;
	return DEVICE_ERROR_NONE;
}

int device_battery_is_charging(bool *charging)
{
	int ret, val;

	if (!charging)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, &val);
	/* regard not supported as disconnected */
	if (val == -ENOTSUP)
		val = 0;
	else if (ret < 0 || val < 0 || val > 1)
		return DEVICE_ERROR_OPERATION_FAILED;

	*charging = val;
	return DEVICE_ERROR_NONE;
}

int device_battery_get_level_status(device_battery_level_e *status)
{
	int val, ret;

	if (!status)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_LEVEL_STATUS, &val);
	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	if (val == VCONFKEY_SYSMAN_BAT_LEVEL_EMPTY)
		*status = DEVICE_BATTERY_LEVEL_EMPTY;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_CRITICAL)
		*status = DEVICE_BATTERY_LEVEL_CRITICAL;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_LOW)
		*status = DEVICE_BATTERY_LEVEL_LOW;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_HIGH)
		*status = DEVICE_BATTERY_LEVEL_HIGH;
	else if (val == VCONFKEY_SYSMAN_BAT_LEVEL_FULL)
		*status = DEVICE_BATTERY_LEVEL_FULL;
	/* regard not supported as disconnected */
	else if (val == -ENOTSUP)
		*status = DEVICE_BATTERY_LEVEL_EMPTY;
	else
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}

static int device_battery_get_info(struct battery_info *info)
{
	int ret;
	GVariant *output = NULL;
	char *status = NULL;
	char *health = NULL;
	char *power_source;
	int online;
	int present;
	int capacity;
	int current_now;
	int current_average;

	if (!info)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = dbus_method_sync_with_reply(DEVICED_BUS_NAME,
			DEVICED_PATH_BATTERY, DEVICED_INTERFACE_BATTERY,
			METHOD_GET_INFO, NULL, NULL, &output);
	/* regard not suppoted as disconnected */
	if (ret == -ENOTSUP)
		ret = 0;
	else if (ret < 0)
		return errno_to_device_error(ret);

	g_variant_get(output, "(isssiiiii)", &ret,
			&status, &health, &power_source,
			&online, &present, &capacity,
			&current_now, &current_average);

	if (ret < 0) {
		ret = errno_to_device_error(ret);
		goto out;
	}

	snprintf(info->status, sizeof(info->status), "%s", status);
	snprintf(info->health, sizeof(info->health), "%s", health);
	snprintf(info->power_source, sizeof(info->power_source), "%s", power_source);
	info->online = online;
	info->present = present;
	info->capacity = capacity;
	info->current_now = current_now;
	info->current_average = current_average;

	ret = DEVICE_ERROR_NONE;

out:
	free(status);
	free(health);
	free(power_source);
	g_variant_unref(output);

	return ret;
}

int device_battery_get_health(device_battery_health_e *health)
{
	struct battery_info info;
	int ret;
	size_t len;

	ret = device_battery_get_info(&info);
	if (ret != DEVICE_ERROR_NONE) {
		_E("Failed to get battery info (%d)", ret);
		return ret;
	}

	len = strlen(info.status);
	if (!strncmp(info.health, "Good", len))
		*health = DEVICE_BATTERY_HEALTH_GOOD;
	else if (!strncmp(info.health, "Cold", len))
		*health = DEVICE_BATTERY_HEALTH_COLD;
	else if (!strncmp(info.health, "Dead", len))
		*health = DEVICE_BATTERY_HEALTH_DEAD;
	else if (!strncmp(info.health, "Overheat", len))
		*health = DEVICE_BATTERY_HEALTH_OVER_HEAT;
	else if (!strncmp(info.health, "Over voltage", len))
		*health = DEVICE_BATTERY_HEALTH_OVER_VOLTAGE;
	else
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}

int device_battery_get_power_source(device_battery_power_source_e *source)
{
	struct battery_info info;
	int ret;
	size_t len;

	ret = device_battery_get_info(&info);
	if (ret != DEVICE_ERROR_NONE) {
		_E("Failed to get battery info (%d)", ret);
		return ret;
	}

	len = strlen(info.status);
	if (!strncmp(info.power_source, "ac", len))
		*source = DEVICE_BATTERY_POWER_SOURCE_AC;
	else if (!strncmp(info.power_source, "usb", len))
		*source = DEVICE_BATTERY_POWER_SOURCE_USB;
	else if (!strncmp(info.power_source, "wireless", len))
		*source = DEVICE_BATTERY_POWER_SOURCE_WIRELESS;
	else
		*source = DEVICE_BATTERY_POWER_SOURCE_NONE;

	return DEVICE_ERROR_NONE;
}

int device_battery_get_property(device_battery_property_e property, int *val)
{
	struct battery_info info;
	int ret;

	if (!val)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = device_battery_get_info(&info);
	if (ret != DEVICE_ERROR_NONE) {
		_E("Failed to get battery info (%d)", ret);
		return ret;
	}

	switch (property) {
	case DEVICE_BATTERY_PROPERTY_CAPACITY:
		*val = info.capacity;
		break;
	case DEVICE_BATTERY_PROPERTY_CURRENT_NOW:
		*val = info.current_now;
		break;
	case DEVICE_BATTERY_PROPERTY_CURRENT_AVERAGE:
		*val = info.current_average;
		break;
	default:
		return DEVICE_ERROR_INVALID_PARAMETER;
	}

	return DEVICE_ERROR_NONE;
}

int device_battery_get_status(device_battery_status_e *status)
{
	struct battery_info info;
	int ret;
	size_t len;

	ret = device_battery_get_info(&info);
	if (ret != DEVICE_ERROR_NONE) {
		_E("Failed to get battery info (%d)", ret);
		return ret;
	}

	len = strlen(info.status);
	if (!strncmp(info.status, "Charging", len))
		*status = DEVICE_BATTERY_STATUS_CHARGING;
	else if (!strncmp(info.status, "Discharging", len))
		*status = DEVICE_BATTERY_STATUS_DISCHARGING;
	else if (!strncmp(info.status, "Full", len))
		*status = DEVICE_BATTERY_STATUS_FULL;
	else if (!strncmp(info.status, "Not charging", len))
		*status = DEVICE_BATTERY_STATUS_NOT_CHARGING;
	else
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}
