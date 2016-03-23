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
#include <system_info.h>

#include "ir.h"
#include "common.h"
#include "dbus.h"

#define METHOD_IS_AVAILABLE		"IRIsAvailable"
#define METHOD_TRANSMIT			"TransmitIR"
#define IR_FEATURE			"http://tizen.org/feature/consumer_ir"


int device_ir_is_available(bool *available)
{
	int ret;
	bool ir_avail;

	if (!available)
		return DEVICE_ERROR_INVALID_PARAMETER;

	ret = system_info_get_platform_bool(IR_FEATURE, &ir_avail);

	if (ret < 0) {
		*available = false;
		return DEVICE_ERROR_OPERATION_FAILED;
	} else if (ret == 0 && !ir_avail) {
		*available = false;
		return DEVICE_ERROR_NOT_SUPPORTED;
	}

	ret = dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_IR,
			DEVICED_INTERFACE_IR, METHOD_IS_AVAILABLE,
			NULL, NULL);

	if (!ret) {
		*available = false;
		return DEVICE_ERROR_OPERATION_FAILED;
	}

	*available = ir_avail;
	return DEVICE_ERROR_NONE;
}

int device_ir_transmit(int carrier_frequency, int *pattern, int size)
{
	char *arr[1];
	struct dbus_int pattern_list;
	int freq_pattern[size + 1];
	int ret;
	int i;
	bool ir_avail;

	if (!pattern)
		return DEVICE_ERROR_INVALID_PARAMETER;
	if (size <= 0) {
		_E("IR pattern size is invalid");
		return DEVICE_ERROR_INVALID_PARAMETER;
	}

	ret = device_ir_is_available(&ir_avail);
	if (!ir_avail) {
		if (ret < 0) {
			_E("IR is not supported or IR operation failed");
			return ret;
		}
		_E("IR is not supported");
		return DEVICE_ERROR_OPERATION_FAILED;
	}

	freq_pattern[0] = carrier_frequency;
	for (i = 1; i <= size; i++)
		freq_pattern[i] = pattern[i-1];

	pattern_list.list = freq_pattern;
	pattern_list.size = size + 1;
	arr[0] = (char *)&pattern_list;

	ret = dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_IR,
			DEVICED_INTERFACE_IR, METHOD_TRANSMIT,
			"ai", arr);

	if (ret < 0)
		return DEVICE_ERROR_OPERATION_FAILED;

	return DEVICE_ERROR_NONE;
}
