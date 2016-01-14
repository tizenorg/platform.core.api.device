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
#include <stdbool.h>

#include "ir.h"

int device_ir_is_available(bool *available)
{
	*available = false;
	return DEVICE_ERROR_NONE;
}

int device_ir_transmit(int carrier_frequency, int *pattern, int size)
{
	return DEVICE_ERROR_NONE;
}

int device_ir_get_available_frequency(int *carrier_frequency, int size)
{
	carrier_frequency = NULL;
	size = 0;
	return DEVICE_ERROR_NONE;
}
