/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "ir.h"
#include "common.h"
#include "dbus.h"

#define METHOD_IS_AVAILABLE		"IRisAvailable"
#define METHOD_TRANSMIT			"TransmitIR"

int device_ir_is_available(bool *available)
{
	int ret;
	*available = false;

	ret = dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_IR,
			DEVICED_INTERFACE_IR, METHOD_IS_AVAILABLE,
			NULL, NULL);

	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	*available = ret;
	return DEVICE_ERROR_NONE;
}

int device_ir_transmit(int carrier_frequency, int *pattern, int size)
{
	char *arr[1];
	struct dbus_int pattern_list;
	int ret;

	pattern_list.freq = carrier_frequency;
	pattern_list.list = pattern;
	pattern_list.size = size;
	arr[0] = (char *)&pattern_list;

	ret = dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_IR,
			DEVICED_INTERFACE_IR, METHOD_TRANSMIT,
			"ai", arr);

	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}
