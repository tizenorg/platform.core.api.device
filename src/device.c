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
#include <string.h>
#include <errno.h>
#include <vconf.h>

#include "device.h"
#include "common.h"
#include "display.h"

#define CHECK_ERR(val)	\
	do {	\
		if (val < 0) {		\
			if (errno == ENODEV)	\
				return DEVICE_ERROR_NOT_SUPPORTED;	\
			return DEVICE_ERROR_OPERATION_FAILED;	\
		}	\
	} while (0)

//LCOV_EXCL_START
int device_get_display_numbers(int* device_number)
{
	return device_display_get_numbers(device_number);
}

int device_get_brightness(int disp_idx, int* value)
{
	return device_display_get_brightness(disp_idx, value);
}

int device_set_brightness(int disp_idx, int new_value)
{
	_E("Deprecated api.");
	return DEVICE_ERROR_NOT_SUPPORTED;
}

int device_get_max_brightness(int disp_idx, int* max_value)
{
	return device_display_get_max_brightness(disp_idx, max_value);
}

int device_set_brightness_from_settings(int disp_idx)
{
	_E("Deprecated api.");
	return DEVICE_ERROR_NOT_SUPPORTED;
}

int device_set_brightness_to_settings(int disp_idx, int new_value)
{
	return device_display_set_brightness(disp_idx, new_value);
}
//LCOV_EXCL_STOP
